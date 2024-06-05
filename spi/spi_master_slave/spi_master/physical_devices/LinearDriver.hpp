#ifndef PICO_EXAMPLES_LINEARDRIVER_HPP
#define PICO_EXAMPLES_LINEARDRIVER_HPP
#include <cstring>
#include "../utilities/base_types/Decoder.hpp"
#include "../loop/common_data/device_variables.hpp"
#include "../loop/common_data/common_variables.hpp"   

class LinearDriverBase
{
 protected:
  OutputPort *z_a;
  OutputPort *z_b;
 public:
   LinearDriverBase();
   virtual ~LinearDriverBase();
   virtual void activate(int command, int freq, int p, int n, bool dir) const;
};

class LinearDriverPico2040: public LinearDriverBase
{protected: 
  bool    _flgOnlyZ; 
  ConfigLinearDrive _configlineardrive;
  OutputPort *y_a;
  OutputPort *y_b;
  OutputPort *x_a;
  OutputPort *x_b;
 public:
   LinearDriverPico2040(bool flgOnlyZ,ConfigLinearDrive configlineardrive);  
  ~LinearDriverPico2040();
   void activate(int command, int freq, int duty, int n, bool dir) const override;
};

class LinearDriverMotherBoard: public LinearDriverBase
{
 private:
    OutputPort *turnon_x;
    OutputPort *turnon_y;
    OutputPort *turnon_z ;
 protected:
  ConfigLinearDriveNew _configlineardrive;
 public:
   LinearDriverMotherBoard(ConfigLinearDriveNew configlineardrive);
   ~LinearDriverMotherBoard();
   void activate(int command, int freq, int p, int n, bool dir) const override;
};
#endif
