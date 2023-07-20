#include <cstdio>
#include <pico/multicore.h>
#include "pico/stdlib.h"
#include "Parser.h"
#include "Spi.hpp"
#include "Decoder.hpp"
#include "Utilities.hpp"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "peripheral_functions.hpp"
#include "LinearDriver.hpp"
#include "ad5664.hpp"
#include <unistd.h>
//#include "hardware/uart.h"

int main()
{
    setDefaultSettings();
    /// MAIN LOOP
    while (true)
    {
        if (LID)
        {
            LID = false;
            static uint16_t inBuf[5]; // f, p, n, d
            for (int j = 0; j < 5; ++j)
            {
                inBuf[j] = vector[j];
                std::cout << inBuf[j] << ' ';
            }
            linearDriver.activate(inBuf[0], inBuf[1], inBuf[2], inBuf[3], inBuf[4]);
            std::cout << "LID_IS_READY\n";
            continue;
        }
        if (AD7606_IS_SCANNING)
        {
            static uint16_t inBuf[5]; // n, start_freq, step, channel, delay
            if (!is_already_scanning)
            {
                is_already_scanning = true;
                for (int j = 0; j < 5; ++j)
                {
                    inBuf[j] = vector[1 + j];
                }
                current_channel = inBuf[3] - 1;
            }
            else
            {
                if (scan_index++ < inBuf[0] && !AD7606_STOP_SCAN)
                {
                    set_freq(inBuf[1]);
                    sleep_ms(inBuf[4]);
                    get_result_from_adc();
                    sleep_ms(10);
                    inBuf[1] += inBuf[2];
                }
                else
                {
                    std::cout << afc << '\n';
                    afc.clear();
                    scan_index = current_freq = 0;
                    current_channel = -1;
                    AD7606_IS_SCANNING = AD7606_STOP_SCAN = is_already_scanning = false;
                }
            }
            continue;
        }
        if (AD9833_SET_FREQ)
        {
//            set_clock_enable();
            AD9833_SET_FREQ = false;
            set_freq(vector[1]);
        }
        if (AD8400_SET_GAIN)
        {
            AD8400_SET_GAIN = false;
            set_gain(vector[1]);
        }
        if (AD7606_GET_VALUE)
        {
//            set_clock_enable();
            AD7606_GET_VALUE = false;
            current_channel = vector[1];
            get_result_from_adc();
            sleep_ms(10);
            continue;
        }
        if (MICRO_SCAN)
        {
            MICRO_SCAN = false;
        }

        /// MAIN IF
        decoder.activePort(vector[1]);
        Spi::setProperties(vector[2], vector[3], vector[4]);
        if (AD5664)
        {
            AD5664 = false;
            AD56X4Class::setChannel(0, AD56X4_SETMODE_INPUT, vector[6], vector[5]);
            AD56X4Class::updateChannel(0, vector[6]);
        }
        if (AD9833_SENDER)
        {
            AD9833_SENDER = false;
            uint8_t buf[6];
            for (int j = 0; j < 6; ++j)
            {
                buf[j] = vector[5 + j];
            }
            spi_write_blocking(spi_default, buf, 6);
        }
        if (AD8400_SENDER)
        {
            AD8400_SENDER = false;
            uint8_t inBuf[1];
            inBuf[0] = vector[5];
            spi_write_blocking(spi_default, inBuf, 1);
        }
        if (AD7606_ENABLE_DISABLE)
        {
            AD7606_ENABLE_DISABLE = false;
            if (vector[5] == 1)
            {
                AD7606_READ_FOREVER = false;
            }
            else if (vector[5] == 0)
            {
                AD7606_READ_FOREVER = true;
            }
        }
        if (AD7606_RESET)
        {
            AD7606_RESET = false;
            resetPort.enable();
            sleep_us(10);
            resetPort.disable();
        }
        if (AD7606_READ or AD7606_READ_FOREVER)
        {
            AD7606_READ = false;
//            set_clock_enable();
            get_result_from_adc();
            sleep_ms(10);
        }
    }
}
