#ifndef PICO_EXAMPLES_LINEARDRIVER_HPP
#define PICO_EXAMPLES_LINEARDRIVER_HPP

#include "Decoder.hpp"

class LinearDriver
{
public:
  LinearDriver(int portA, int portB);

  void activate(int freq, int p, int n, bool dir);


private:
  GpioPort a_;
  GpioPort b_;
};


#endif
