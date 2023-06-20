#ifndef PICO_EXAMPLES_LINEARDRIVER_HPP
#define PICO_EXAMPLES_LINEARDRIVER_HPP

#include "Decoder.hpp"

class LinearDriver
{
public:
  LinearDriver();

  void activate(int command, int freq, int p, int n, bool dir);

private:
  GpioPort x_a;
  GpioPort x_b;
  GpioPort y_a;
  GpioPort y_b;
  GpioPort z_a;
  GpioPort z_b;
};


#endif
