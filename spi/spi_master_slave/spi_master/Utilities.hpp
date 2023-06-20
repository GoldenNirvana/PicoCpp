#ifndef PICO_EXAMPLES_UTILITIES_HPP
#define PICO_EXAMPLES_UTILITIES_HPP

#include <iostream>
#include <iomanip>
#include <pico/bootrom.h>
#include "LinearDriver.hpp"
#include "pico/mutex.h"

mutex mut;
Spi spi;
Decoder decoder(4, 5, 6);

static uint16_t spiBuf[8];
static std::string afc;
static int vector[15];
static int vectorSize;

bool AD9833_SENDER = false;
bool AD8400_SENDER = false;
bool AD7606_ENABLE_DISABLE = false;
bool AD7606_RESET = false;
bool AD7606_READ = false;
bool AD5664_SENDER = false;
bool AD7606_READ_FOREVER = false;
bool AD7606_STOP_SCAN = false;

bool LID = false;

bool AD7606_IS_SCANNING = false;
bool is_already_scanning = false;
uint16_t scan_index = 0;
uint16_t current_freq = 0;
//int current_receive = 0;
int current_channel = 0;

GpioPort conv(7);
GpioPort dec(10);
GpioPort resetPort(8);
GpioPort ledPort(PICO_DEFAULT_LED_PIN);

LinearDriver linearDriver;

[[noreturn]] void activateError();

void serialPrintBuffer(const uint16_t *buf, int len);

void parse(int *vector);

void comReceiveISR(uint a, uint32_t b);

void launchOnCore1();

void resetAll();

void comReceiveISR(uint a, uint32_t b)
{
    spi_read16_blocking(spi_default, 0, spiBuf, 8);
    if (is_already_scanning)
    {
        afc += std::to_string(current_freq) + ',' + std::to_string(spiBuf[current_channel]) + ',';
    } else
    {
        serialPrintBuffer(spiBuf, 8);
    }
}

void launchOnCore1()
{
    while (true)
    {
        /// PARSING
        parse(vector);
        switch (vector[0])
        {
            case 1:
                AD9833_SENDER = true;
                break;
            case 5:
                AD8400_SENDER = true;
                break;
            case 6:
                AD7606_ENABLE_DISABLE = true;
                break;
            case 11:
                AD7606_RESET = true;
                break;
            case 12:
                AD7606_READ = true;
                break;
            case 20:
                AD5664_SENDER = true;
                break;
            case 25:
                AD7606_IS_SCANNING = true;
                break;
            case 26:
                AD7606_STOP_SCAN = true;
                break;
            case 100 ... 110:
                LID = true;
                break;
            default:
                activateError();
        }
    }
}


void serialPrintBuffer(const uint16_t *const buf, int len)
{
    uint32_t x = time_us_64();
    std::cout << "[" << std::fixed << std::setfill('0') << std::setw(15) << std::right << x << "]     ";
    std::cout << std::resetiosflags(std::ios_base::right);
    std::cout << std::resetiosflags(std::ios_base::fixed);
    for (int i = 0; i < len; ++i)
    {
        std::cout << buf[i] << ' ';
    }
    std::cout << "\n";
}

void serialPrintBuffer(const uint8_t *const buf, int len)
{
    uint32_t x = time_us_64();
    std::cout << "[" << std::fixed << std::setfill('0') << std::setw(15) << std::right << x << "]     ";
    std::cout << std::resetiosflags(std::ios_base::right);
    std::cout << std::resetiosflags(std::ios_base::fixed);
    for (int i = 0; i < len; ++i)
    {
        std::cout << buf[i] << ' ';
    }
    std::cout << "\n";
}


void parse(int *vec)
{
    std::string s;
    getline(std::cin, s);
    Parser parser(s.data(), ',');
    for (int c = 0; c < 10; c++)
    {
        vec[c] = -1;
    }
    vectorSize = parser.parseInts(vec);
}

void activateError()
{
    while (true)
    {
        ledPort.enable();
        sleep_ms(100);
        ledPort.disable();
        sleep_ms(100);
    }
}

void resetAll()
{
    AD9833_SENDER = false;
    AD8400_SENDER = false;
    AD7606_ENABLE_DISABLE = false;
    AD7606_RESET = false;
    AD7606_READ = false;
    AD5664_SENDER = false;
    AD7606_READ_FOREVER = false;
    AD7606_IS_SCANNING = false;
}

void setDefaultSettings()
{
    /// BASIC SETTINGS
    mutex_init(&mut);
    if (!mutex_is_initialized(&mut))
    {
        activateError();
    }
    int port = 9; // Busy port
    gpio_init(port);
    gpio_set_dir(port, GPIO_IN);
    gpio_pull_down(resetPort.getPort());
    gpio_set_irq_enabled_with_callback(port, GPIO_IRQ_EDGE_FALL, true, comReceiveISR);

    multicore_launch_core1(launchOnCore1);

    dec.enable();
    conv.enable();
    resetPort.disable();
    gpio_pull_down(resetPort.getPort());
    ledPort.enable();
}

#endif
