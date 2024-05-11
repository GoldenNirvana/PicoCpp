#ifndef PICO_EXAMPLES_LINEARDRIVER_HPP
#define PICO_EXAMPLES_LINEARDRIVER_HPP
#include <cstring>
#include "../utilities/base_types/Decoder.hpp"
#include "../loop/common_data/device_variables.hpp"
#include "../loop/common_data/common_variables.hpp"   //add mf 231101 240505


class LinearDriver
{
 private:
  bool    _flgOnlyZ; 
  ConfigLinearDrive _configlineardrive;
  OutputPort *x_a;
  OutputPort *x_b;
  OutputPort *y_a;
  OutputPort *y_b;
  OutputPort *z_a;
  OutputPort *z_b;
 public:
  LinearDriver(bool flgOnlyZ,ConfigLinearDrive configlineardrive);
 ~LinearDriver();
  void activate(int command, int freq, int p, int n, bool dir);
};


#endif
