#include "LinearDriver.hpp"

LinearDriver::LinearDriver(int portA, int portB) : a_(GpioPort(portA)), b_(GpioPort(portB))
{}


void LinearDriver::activate()
{

}

