#include "rx_core.hpp"

#include <iostream>
#include <iomanip>
#include <pico/bootrom.h>
#include "../physical_devices/LinearDriver.hpp"
#include "pico/mutex.h"
#include "../Parser.h"
#include "../Spi.hpp"
#include <pico/multicore.h>
#include "../common_variables.hpp"

extern std::string afc;

void comReceiveISR(uint a, uint32_t b) {
    // set settings here mb
    spi_read16_blocking(spi_default, 0, spiBuf, 8);
    if (is_already_scanning) {
        afc += std::to_string(current_freq) + ',' + std::to_string(spiBuf[current_channel]) + ',';
    } else if (AD7606_TRIG_GET_VALUE) // cond other way
    {
        AD7606_TRIG_GET_VALUE = false;
        critical_section_enter_blocking(&criticalSection);
        if (current_channel == -1) {
            std::cout << "ERROR\n";
        }
        std::cout << spiBuf[current_channel] << '\n';
        current_channel = -1;
        critical_section_exit(&criticalSection);
    } else if (AD7606_GET_ALL_VALUES) {
        AD7606_GET_ALL_VALUES = false;
        serialPrintBuffer(spiBuf, 8);
    }
}

void launchOnCore1() {
    while (true) {
//        mutex_enter_timeout_ms(&mut, 10);
//        std::cout << "Mutex captured by core1 << '\n";
        /// PARSING
        parse(vector);
        switch (vector[0]) {
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
            case 21:
                AD5664 = true;
                break;
            case 24:
                AD7606_GET_VALUE = true;
                break;
            case 25:
                AD7606_IS_SCANNING = true;
                break;
            case 26:
                AD7606_STOP_SCAN = true;
                break;
            case 30:
                AD9833_SET_FREQ = true;
                break;
            case 40:
                AD8400_SET_GAIN = true;
                break;
            case 50:
                MICRO_SCAN = true;
                break;
            case 51:
                MOVE_TO = true;
                break;
            case 52:
                STOP_MICRO_SCAN = true;
                break;
            case 55:
                CONFIG_UPDATE = true;
                break;
            case 60:
                SET_IO_VALUE = true;
                break;
            case 61:
                SET_ONE_IO_VALUE = true;
                break;
            case 100 ... 110:
                LID = true;
                break;
            default:
                activateError();
        }
//        mutex_exit(&mut);
//        std::cout << "Mutex released by core1\n";
    }
}

void serialPrintBuffer(const uint16_t *const buf, int len) {
    uint32_t x = time_us_64();
    uint64_t a = time_us_64();
    std::cout << "[" << std::fixed << std::setfill('0') << std::setw(15) << std::right << x << "_u32 " << a
              << "_u64]     ";
    std::cout << std::resetiosflags(std::ios_base::right);
    std::cout << std::resetiosflags(std::ios_base::fixed);
    for (int i = 0; i < len; ++i) {
        std::cout << buf[i] << ' ';
    }
    std::cout << "\n";
}

void serialPrintBuffer(const uint8_t *const buf, int len) {
    uint32_t x = time_us_64();
    std::cout << "[" << std::fixed << std::setfill('0') << std::setw(15) << std::right << x << "]     ";
    std::cout << std::resetiosflags(std::ios_base::right);
    std::cout << std::resetiosflags(std::ios_base::fixed);
    for (int i = 0; i < len; ++i) {
        std::cout << buf[i] << ' ';
    }
    std::cout << "\n";
}

void parse(int32_t *vec) {
    std::string s;
    getline(std::cin, s);
    Parser parser(const_cast<char *>(s.data()), ',');
    for (int c = 0; c < 10; c++) {
        vec[c] = -1;
    }
    vectorSize = parser.parseInts(vec);
}

void activateError() {
    while (true) {
        ledPort.enable();
        sleep_ms(100);
        ledPort.disable();
        sleep_ms(100);
    }
}

void resetAll() {
    AD9833_SENDER = false;
    AD8400_SENDER = false;
    AD7606_ENABLE_DISABLE = false;
    AD7606_RESET = false;
    AD7606_READ = false;
    AD7606_READ_FOREVER = false;
    AD7606_IS_SCANNING = false;
}

void setDefaultSettings() {
    /// BASIC SETTINGS
    gpio_pull_down(resetPort.getPort());
    // fixme mb should add & before isr
    gpio_set_irq_enabled_with_callback(busy.getPort(), GPIO_IRQ_EDGE_FALL, true, comReceiveISR);

    multicore_launch_core1(launchOnCore1);

    dec.enable();
    conv.enable();
    resetPort.disable();
    gpio_pull_down(resetPort.getPort());
    ledPort.enable();
}