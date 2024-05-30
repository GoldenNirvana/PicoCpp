#ifndef PICO_EXAMPLES_LINEARDRIVER_HPP
#define PICO_EXAMPLES_LINEARDRIVER_HPP
#include <cstring>
#include "../utilities/base_types/Decoder.hpp"
#include "../loop/common_data/device_variables.hpp"
#include "../loop/common_data/common_variables.hpp"   //add mf 231101 240505

class LinearDriverBase
{
 protected:
  OutputPort *z_a;
  OutputPort *z_b;
  /*
  OutputPort *y_a;
  OutputPort *y_b;
  OutputPort *x_a;
  OutputPort *x_b;
  */
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
   void activate(int command, int freq, int p, int n, bool dir) const override;
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
   LinearDriverMotherBoard(ConfigLinearDriveNew configlineardrive);// 
   ~LinearDriverMotherBoard();
  void activate(int command, int freq, int p, int n, bool dir) const override;
};

/*


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
  //LinearDriverBase(bool flgOnlyZ,ConfigLinearDrive configlineardrive);
  ~LinearDriverBase();
  void activate(int command, int freq, int p, int n, bool dir);
};

class LinearDriverPico2040: public LinearDriverBase
{
 public:
  LinearDriverPico2040(bool flgOnlyZ,ConfigLinearDrive configlineardrive);//:LinearDriverBase(flgOnlyZ,configlineardrive);
 // ~LinearDriverPico2040();
  void activate(int command, int freq, int p, int n, bool dir);
};

class LinearDriverMotherBoard: public LinearDriverBase
{
 public:
   LinearDriverMotherBoard(bool flgOnlyZ,ConfigLinearDrive configlineardrive);// 
 //    ~LinearDriverMotherBoard();
  void activate(int command, int freq, int p, int n, bool dir);
};
*/
#endif
