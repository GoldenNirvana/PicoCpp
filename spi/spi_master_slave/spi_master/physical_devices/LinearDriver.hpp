#ifndef PICO_EXAMPLES_LINEARDRIVER_HPP
#define PICO_EXAMPLES_LINEARDRIVER_HPP
#include <cstring>
#include "../utilities/base_types/Decoder.hpp"
#include "../loop/common_data/device_variables.hpp"
#include "../loop/common_data/common_variables.hpp"   //add mf 231101 240505


class LinearDriverBase
{
 protected:
  bool    _flgOnlyZ; 
  ConfigLinearDrive _configlineardrive;
  OutputPort *x_a;
  OutputPort *x_b;
  OutputPort *y_a;
  OutputPort *y_b;
  OutputPort *z_a;
  OutputPort *z_b;
 public:
  LinearDriverBase(bool flgOnlyZ,ConfigLinearDrive configlineardrive);
  virtual ~LinearDriverBase();
  void activate(int command, int freq, int p, int n, bool dir);
};
/*
class LinearDriverPico2040: public LinearDriverBase
{
 public:
  LinearDriverPico2040(bool flgOnlyZ,ConfigLinearDrive configlineardrive);//:LinearDriverBase(flgOnlyZ,configlineardrive){};
  ~LinearDriverPico2040();
 void activate(int command, int freq, int p, int n, bool dir);
};

class LinearDriverMotherBoard: public LinearDriverBase
{
 public:
   LinearDriverMotherBoard(bool flgOnlyZ,ConfigLinearDrive configlineardrive);// 
  // :LinearDriverBase(flgOnlyZ,configlineardrive){};
  ~LinearDriverMotherBoard();
 void activate(int command, int freq, int p, int n, bool dir);
};
*/
#endif
