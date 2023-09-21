#include "main_core.hpp"
#include <bitset>
#include <iostream>
#include "../utilities/peripheral_functions.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "common_data/common_variables.hpp"

void MainCore::loop()
{

    while (true)
    {
        if (MOVE_TO)
        {
            MOVE_TO = false;
            scanner.move_to({static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2])}, vector[3]);
            continue;
        }
        if (MICRO_SCAN || CONFIG_UPDATE)
        {
            if (CONFIG_UPDATE)
            {
                CONFIG_UPDATE = false;
                scanner.update({static_cast<uint32_t>(vector[1]), static_cast<uint32_t>(vector[2]),
                                static_cast<uint8_t>(vector[3]), static_cast<uint8_t>(vector[4]),
                                static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
                                static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
                                static_cast<uint8_t>(vector[9])});
                continue;
            }
            if (MICRO_SCAN)
            {
                MICRO_SCAN = false;
                scanner.start_scan({static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2])});
            }
        }
        if (SET_IO_VALUE)
        {
            SET_IO_VALUE = false;
            if (vector[1] == 1)
            {
                std::string binary = std::bitset<2>(vector[2]).to_string();
                binary[1] == '1' ? io1_0.enable() : io1_0.disable();
                binary[0] == '1' ? io1_1.enable() : io1_1.disable();
            } else if (vector[1] == 2)
            {
                std::string binary = std::bitset<3>(vector[2]).to_string();
                binary[2] == '1' ? io2_0.enable() : io2_0.disable();
                binary[1] == '1' ? io2_1.enable() : io2_1.disable();
                binary[0] == '1' ? io2_2.enable() : io2_2.disable();
            } else if (vector[1] == 3)
            {
                std::string binary = std::bitset<2>(vector[2]).to_string();
                binary[1] == '1' ? io3_0.enable() : io3_0.disable();
                binary[0] == '1' ? io3_1.enable() : io3_1.disable();
            }
        }
        if (SET_ONE_IO_VALUE)
        {
            SET_ONE_IO_VALUE = false;
            static std::vector<OutputPort> io_ports;
            io_ports.push_back(io1_0);
            io_ports.push_back(io1_1);
            io_ports.push_back(io2_0);
            io_ports.push_back(io2_1);
            io_ports.push_back(io2_2);
            io_ports.push_back(io3_0);
            io_ports.push_back(io3_1);
            vector[2] == 1 ? io_ports[vector[1] - 1].enable() : io_ports[vector[1] - 1].disable();
        }
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
            } else
            {
                if (scan_index++ < inBuf[0] && !AD7606_STOP_SCAN)
                {
                    set_freq(inBuf[1]);
                    sleep_ms(inBuf[4]);
                    get_result_from_adc();
                    sleep_ms(10);
                    inBuf[1] += inBuf[2];
                } else
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
            set_freq((uint32_t) vector[1]);
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
            AD7606_TRIG_GET_VALUE = true;
            critical_section_enter_blocking(&criticalSection);
            current_channel = vector[1];  // выводить значение CUURENT CHANNEL
            critical_section_exit(&criticalSection);
            get_result_from_adc();
            continue;
        }


        /// MAIN SPI IF
        decoder.activePort(vector[1]);
        Spi::setProperties(vector[2], vector[3], vector[4]);

        if (DAC8563_SET_VOLTAGE)
        {
            DAC8563_SET_VOLTAGE = false;
            if (vector[5] == 0)
            {
                dac8563.writeA(vector[6]);
            } else if (vector[5] == 1)
            {
                dac8563.writeB(vector[6]);
            }
        }

        if (AD5664)
        {
            AD5664 = false;
            AD56X4Class::setChannel(AD56X4_SETMODE_INPUT, vector[6], vector[5]);
            AD56X4Class::updateChannel(vector[6]);
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
            } else if (vector[5] == 0)
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
            AD7606_GET_ALL_VALUES = true;
//            set_clock_enable();
            get_result_from_adc();
        }
    }

}

MainCore::MainCore()
{

}
