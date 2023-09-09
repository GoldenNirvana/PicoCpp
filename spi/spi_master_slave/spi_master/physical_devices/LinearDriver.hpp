#ifndef PICO_EXAMPLES_LINEARDRIVER_HPP
#define PICO_EXAMPLES_LINEARDRIVER_HPP

#include "../utilities/base_types/Decoder.hpp"

class LinearDriver
{
public:
  LinearDriver();

  void activate(int command, int freq, int p, int n, bool dir);

private:
  OutputPort x_a;
  OutputPort x_b;
  OutputPort y_a;
  OutputPort y_b;
  OutputPort z_a;
  OutputPort z_b;
};


#endif
