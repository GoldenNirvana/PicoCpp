#include <algorithm>
#include "LinearDriver.hpp"

LinearDriver::LinearDriver(int portA, int portB) : a_(GpioPort(portA)), b_(GpioPort(portB))
{
  a_.disable();
  b_.disable();
}


void LinearDriver::activate(int freq, int p, int n, bool dir)
{
  double t_abs = 1000 / freq;
  double t_low = p * t_abs / 1000;
  double t_high = t_abs - t_low;

  if (dir)
  {
    std::swap(a_, b_);
  }

  a_.enable();
  for (int i = 0; i < n; ++i)
  {
    b_.disable();
    sleep_ms(t_low);
    b_.enable();
    sleep_ms(t_high);
  }
  b_.disable();
  a_.disable();

  if (dir)
  {
    std::swap(a_, b_);
  }
}

