#ifndef PICO_EXAMPLES_LINEARDRIVER_HPP
#define PICO_EXAMPLES_LINEARDRIVER_HPP

#include "Decoder.hpp"

class LinearDriver
{
public:
  LinearDriver(int portA, int portB);

  void activate();


private:
  GpioPort a_;
  GpioPort b_;
};


#endif
