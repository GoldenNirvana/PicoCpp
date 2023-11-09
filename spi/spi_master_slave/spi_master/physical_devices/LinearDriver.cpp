#include <algorithm>
#include <iostream>
#include "LinearDriver.hpp"
#include "../loop/common_data/common_variables.hpp"//add mf 231101

LinearDriver::LinearDriver() : x_a(OutputPort(18)), x_b(OutputPort(19)), y_a(OutputPort(20)),
                               y_b(OutputPort(21)), z_a(OutputPort(22)), z_b(OutputPort(28))
{
  x_a.disable();
  x_b.disable();
  y_a.disable();
  y_b.disable();
  z_a.disable();
  z_b.disable();
}


void LinearDriver::activate(int command, int freq, int p, int n, bool dir)  ///
{
   if (flgDebugLevel<=DEBUG_LEVEL) std::cout << "From activate command = " << command << '\n';
  OutputPort *ptrA = &x_a;
  OutputPort *ptrB = &x_b;
  if (command == 90)
  {
    ptrA = &x_a;
    ptrB = &x_b;
  }
  if (command == 95)
  {
    ptrA = &y_a;
    ptrB = &y_b;
  } else if (command == 99)
  {
    ptrA = &z_a;
    ptrB = &z_b;
  }
  double t_abs =(double)(1000000 / freq);        // 2000                     // mf 23108
  double t_low =(double)(p * t_abs / 1000);  //  750 * 2000 / 1000000 = 1.5 // mf 23108
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
  ptrA->disable();
  ptrB->disable();

  if (dir)
  {
    std::swap(ptrA, ptrB);
  }
}

