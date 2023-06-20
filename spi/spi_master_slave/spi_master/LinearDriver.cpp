#include <algorithm>
#include <iostream>
#include "LinearDriver.hpp"

LinearDriver::LinearDriver() : x_a(GpioPort(18)), x_b(GpioPort(19)), y_a(GpioPort(20)),
                               y_b(GpioPort(21)), z_a(GpioPort(22)), z_b(GpioPort(28))
{}


void LinearDriver::activate(int command, int freq, int p, int n, bool dir)
{
    std::cout << "From activate command = " <<  command << '\n';
    GpioPort *ptrA = &x_a;
    GpioPort *ptrB = &x_b;
    if (command == 100)
    {
        ptrA = &x_a;
        ptrB = &x_b;
    }
    if (command == 105)
    {
        ptrA = &y_a;
        ptrB = &y_b;
    }
    else if (command == 110)
    {
        ptrA = &z_a;
        ptrB = &z_b;
    }
    double t_abs = 1000 / freq;
    double t_low = p * t_abs / 1000;
    double t_high = t_abs - t_low;

    if (dir)
    {
        std::swap(ptrA, ptrB);
    }

    ptrA->enable();
    for (int i = 0; i < n; ++i)
    {
        ptrB->disable();
        sleep_ms(t_low);
        ptrB->enable();
        sleep_ms(t_high);
    }
    ptrA->disable();
    ptrB->disable();

    if (dir)
    {
        std::swap(ptrA, ptrB);
    }
}

