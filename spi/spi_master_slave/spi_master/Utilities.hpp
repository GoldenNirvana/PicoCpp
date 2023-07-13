#ifndef PICO_EXAMPLES_UTILITIES_HPP
#define PICO_EXAMPLES_UTILITIES_HPP

#include <iostream>
#include <iomanip>
#include <pico/bootrom.h>
#include "LinearDriver.hpp"
#include "pico/mutex.h"

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
bool AD7606_READ_FOREVER = false;
bool AD7606_STOP_SCAN = false;
bool AD9833_SET_FREQ = false;
bool AD8400_SET_GAIN = false;
bool AD7606_GET_VALUE = false;
bool AD5664 = false;
bool AD7606_GET_ON_CHANNEL = false;

bool LID = false;

bool AD7606_IS_SCANNING = false;
volatile bool is_already_scanning = false;
uint16_t scan_index = 0;
uint16_t current_freq = 0;
int current_channel = 0;

InputPort busy(9);
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
    }
    else if (current_channel != -1)
    {
        std::cout << spiBuf[current_channel] << '\n';
        current_channel = -1;
    }
    else
    {
        serialPrintBuffer(spiBuf, 8);
    }
}

void launchOnCore1()
{
    while (true)
    {
//        mutex_enter_timeout_ms(&mut, 10);
//        std::cout << "Mutex captured by core1 << '\n";
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
    AD7606_READ_FOREVER = false;
    AD7606_IS_SCANNING = false;
}

void setDefaultSettings()
{
    /// BASIC SETTINGS
    gpio_pull_down(resetPort.getPort());
    gpio_set_irq_enabled_with_callback(busy.getPort(), GPIO_IRQ_EDGE_FALL, true, comReceiveISR);

    multicore_launch_core1(launchOnCore1);

    dec.enable();
    conv.enable();
    resetPort.disable();
    gpio_pull_down(resetPort.getPort());
    ledPort.enable();
}

#endif
