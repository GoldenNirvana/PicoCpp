#ifndef PICO_EXAMPLES_peripheral_FUNCTIONS_HPP
#define PICO_EXAMPLES_peripheral_FUNCTIONS_HPP

#include <vector>
#include "Spi.hpp"
#include "ad5664.hpp"
#include "Utilities.hpp"


Spi spi;
Decoder decoder(4, 5, 6);

void set_freq(uint16_t freq)
{
    current_freq = freq;

    int64_t flag_freq = 1 << 14;
    int64_t scale = 1 << 28;
    int64_t n_reg = int64_t(freq * scale / 25.0e6);
    int64_t n_low = n_reg & 0x3fff;
    int64_t n_hi = (n_reg >> 14) & 0x3fff;

    static uint8_t buf[6];

    buf[0] = (flag_freq | n_low) / (0x100);
    buf[1] = (flag_freq | n_low) % (0x100);
    buf[2] = (flag_freq | n_hi) / (0x100);
    buf[3] = (flag_freq | n_hi) % (0x100);
    buf[4] = (0x2000) / (0x100);
    buf[5] = (0x2000) % (0x100);

//  serialPrintBuffer(buf, 6);

    decoder.activePort(1);
    Spi::setProperties(8, 1, 1);
    spi_write_blocking(spi_default, buf, 2);
    spi_write_blocking(spi_default, buf + 2, 2);
    spi_write_blocking(spi_default, buf + 4, 2);

}

void get_result_from_adc()
{
    decoder.activePort(0);
    Spi::setProperties(16, 1, 0);
    conv.enable();
    conv.disable();
    sleep_us(10);
    conv.enable();
}

void set_gain(int gain, int p = 2)
{
    uint8_t intBuf[1];
    decoder.activePort(p);
    Spi::setProperties(8, 0, 0);
    intBuf[0] = gain;
    spi_write_blocking(spi_default, intBuf, 3);
}

void set_clock_enable()
{
    uint8_t intBuf[1];
    Spi::setProperties(8, 1, 1);
    decoder.activePort(7);
    spi_write_blocking(spi_default, intBuf, 1);
}

void set_on_cap(uint8_t channel, uint16_t value)
{
    decoder.activePort(3);
    Spi::setProperties(8, 0, 1);
    AD56X4Class::setChannel(AD56X4_SETMODE_INPUT, channel, value);
    AD56X4Class::updateChannel(channel);
}


struct Point
{
    uint16_t x;
    uint16_t y;

    bool operator==(const Point &other) const
    {
        return (x == other.x && y == other.y);
    }

    bool operator!=(const Point &other) const
    {
        return !(*(this) == other);
    }
};

struct Config
{
    uint32_t nPoints_x;  // Точек по линии X
    uint32_t nPoints_y;  // Точек по линии Y
    uint8_t path;  // 0 - по X, 1 - по Y
    uint8_t method;  //
    uint16_t delayF;  // Задержка вперёд
    uint16_t delayB;  // Задержка назад
    uint16_t betweenPoints_x;  // Расстояние между точками по X
    uint16_t betweenPoints_y;  // Расстояние между точками по Y
    uint8_t flag;  // Что измерять
};


class Scanner
{
public:

    Scanner() : pos_({0, 0}), conf_({})
    {}

    ~Scanner()
    {
        move_to({0, 0}, 10);
    }

    void start_scan(const Point &point)
    {
        prev_point = pos_;
        std::cout << conf_.delayB << ' ' << conf_.delayF << '\n';
        move_to(point, 10);
        for (int i = 0; i < conf_.nPoints_y; ++i)
        {
            for (int j = 0; j < conf_.nPoints_x; ++j)
            {
                for (int k = 0; k < conf_.betweenPoints_x; ++k)
                {
                    set_on_cap(1, ++pos_.x);
                    sleep_us(conf_.delayF);
                }
                sleep_ms(50); // CONST 50ms
                get_result_from_adc();
                while (spiBuf[1] == 0);            // TODO ADD others
                vector_z.emplace_back(spiBuf[1]);  // get Z from adc
                if (conf_.flag != 0)
                    other_info.emplace_back(spiBuf[conf_.flag]);
                for (auto &item: spiBuf)
                {
                    item = 0;
                }
            }
            for (int j = 0; j < conf_.betweenPoints_x * conf_.nPoints_x; ++j) // GET back
            {
                set_on_cap(1, --pos_.x);
                sleep_us(conf_.delayB);
            }
            for (int j = 0; j < vector_z.size(); j++)     // send info
            {
                std::cout << vector_z[j] << ' ';
                if (conf_.flag != 0)
                {
                    std::cout << other_info[j] << ';';
                }
            }
            if (STOP_MICRO_SCAN)                     // is need to stop
            {
                STOP_MICRO_SCAN = false;
                stop_scan();
                MICRO_SCAN = false;
                continue;
            }
            for (int j = 0; j < conf_.betweenPoints_y; ++j) // go next line
            {
                set_on_cap(2, ++pos_.y);
                sleep_us(conf_.delayF);
            }
        }
    }

    void start_scan()
    {
        start_scan(pos_);
    }

    void stop_scan()
    {
        move_to(prev_point, 10);
    }

    void update(const Config &config)
    {
        conf_ = config;
    }

    void move_to(const Point &point, uint32_t delay)
    {

        while (pos_.x < point.x)
        {
            set_on_cap(1, ++pos_.x);
            sleep_us(delay);
        }
        while (pos_.x > point.x)
        {
            set_on_cap(1, --pos_.x);
            sleep_us(delay);
        }
        while (pos_.y < point.y)
        {
            set_on_cap(2, ++pos_.y);
            sleep_us(delay);
        }
        while (pos_.y > point.y)
        {
            set_on_cap(2, --pos_.y);
            sleep_us(delay);
        }
        std::cout << "END\n";
    }

private:

    std::vector<uint16_t> vector_z, other_info;
    Point pos_, prev_point;
    Config conf_;
};

Scanner scanner;

#endif
