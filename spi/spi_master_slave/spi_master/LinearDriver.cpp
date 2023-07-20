#include <algorithm>
#include <iostream>
#include "LinearDriver.hpp"

LinearDriver::LinearDriver() : x_a(GpioPort(18)), x_b(GpioPort(19)), y_a(GpioPort(20)),
                               y_b(GpioPort(21)), z_a(GpioPort(22)), z_b(GpioPort(28))
{
    x_a.enable();
    x_b.enable();
    y_a.enable();
    y_b.enable();
    z_a.enable();
    z_b.enable();
}


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
    double t_abs = 1000000 / freq;        // 2000
    double t_low = p * t_abs / 1000;  //  750 * 2000 / 1000000 = 1.5
    double t_high = t_abs - t_low;    // 2 - 1.5 = 0.5

    if (dir)
    {
        std::swap(ptrA, ptrB);
    }

    ptrA->enable();
    for (int i = 0; i < n; ++i)
    {
        ptrB->disable();
        sleep_us(t_low);
        ptrB->enable();
        sleep_us(t_high);
    }
    ptrA->enable();
    ptrB->enable();

    if (dir)
    {
        std::swap(ptrA, ptrB);
    }
}

