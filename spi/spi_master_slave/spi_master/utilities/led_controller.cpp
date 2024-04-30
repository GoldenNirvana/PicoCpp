#include "led_controller.hpp"
#include "../loop/common_data/common_variables.hpp"

void activateError()
{
    while (true)
    {
        ledPort.enable();
        activateRed();
        sleep_ms(1000);
        ledPort.disable();
        activateDark();
        sleep_ms(1000);
    }
}

void green()
{
    int i = 0;
    while (i++ < 3)
    {
        activateDark();
        sleep_ms(100);
        activateDark();
        sleep_ms(100);
        activateDark();
        sleep_ms(100);
        activateGreen();
        sleep_ms(100);
    }
}

void blue()
{
    int i = 0;
    while (i++ < 3)
    {
        activateDark();
        sleep_ms(100);
        activateDark();
        sleep_ms(100);
        activateDark();
        sleep_ms(100);
        activateBlue();
        sleep_ms(100);
    }
}

void red()
{
    int i = 0;
    while (i++ < 3)
    {
        activateDark();
        sleep_ms(100);
        activateDark();
        sleep_ms(100);
        activateDark();
        sleep_ms(100);
        activateRed();
        sleep_ms(100);
    }
}

void dark()
{
    int i = 0;
    while (i++ < 3)
    {
        activateDark();
        sleep_ms(100);
        activateDark();
        sleep_ms(100);
        activateDark();
        sleep_ms(100);
        activateDark();
        sleep_ms(100);
    }
}


void activateGreen()
{
    rdbLed.disable();
    sleep_us(60);
    for (int i = 0; i < 8; ++i)
    {
        rdbLed.enable();
        busy_wait_at_least_cycles(85);
        rdbLed.disable();
        busy_wait_at_least_cycles(35);
    }

    for (int i = 0; i < 16; ++i)
    {
        rdbLed.enable();
        busy_wait_at_least_cycles(35);
        rdbLed.disable();
        busy_wait_at_least_cycles(85);
    }
}

void activateRed()
{
    rdbLed.disable();
    sleep_us(60);
    for (int i = 0; i < 8; ++i)
    {
        rdbLed.enable();
        busy_wait_at_least_cycles(35);
        rdbLed.disable();
        busy_wait_at_least_cycles(85);
    }

    for (int i = 0; i < 8; ++i)
    {
        rdbLed.enable();
        busy_wait_at_least_cycles(85);
        rdbLed.disable();
        busy_wait_at_least_cycles(35);
    }

    for (int i = 0; i < 8; ++i)
    {
        rdbLed.enable();
        busy_wait_at_least_cycles(35);
        rdbLed.disable();
        busy_wait_at_least_cycles(85);
    }
}

void activateBlue()
{
    rdbLed.disable();
    sleep_us(60);
    for (int i = 0; i < 16; ++i)
    {
        rdbLed.enable();
        busy_wait_at_least_cycles(35);
        rdbLed.disable();
        busy_wait_at_least_cycles(85);
    }
    for (int i = 0; i < 8; ++i)
    {
        rdbLed.enable();
        busy_wait_at_least_cycles(85);
        rdbLed.disable();
        busy_wait_at_least_cycles(35);
    }
}

void activateDark()
{
    rdbLed.disable();
    sleep_us(60);
    for (int i = 0; i < 24; ++i)
    {
        rdbLed.enable();
        busy_wait_at_least_cycles(35);
        rdbLed.disable();
        busy_wait_at_least_cycles(85);
    }
}
