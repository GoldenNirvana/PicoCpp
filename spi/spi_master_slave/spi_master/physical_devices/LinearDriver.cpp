#include <algorithm>
#include <iostream>
#include "LinearDriver.hpp"
LinearDriverBase::LinearDriverBase()
{
}
  
LinearDriverBase::~LinearDriverBase()
{
}

LinearDriverPico2040::~LinearDriverPico2040()
{
   delete(x_a);
   delete(x_b);
   delete(y_a);
   delete(y_b);
   delete(z_a);
   delete(z_b);  
}

LinearDriverPico2040::LinearDriverPico2040(bool flgOnlyZ, ConfigLinearDrive configlineardrive)
{
  _configlineardrive=configlineardrive;
  _flgOnlyZ=flgOnlyZ;
  if  (flgDebug)
  {
   afc.clear();
   afc =code+std::to_string(DEBUG)+ "Pico2040 ";
   afc += endln;//"\n";
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  }
   x_a=new OutputPort(_configlineardrive.XA_Port); 
   x_b=new OutputPort(_configlineardrive.XB_Port); 
   y_a=new OutputPort(_configlineardrive.YA_Port); 
   y_b=new OutputPort(_configlineardrive.YB_Port); 
   z_a=new OutputPort(_configlineardrive.ZA_Port); 
   z_b=new OutputPort(_configlineardrive.ZB_Port);  
   x_a->disable();
   x_b->disable();
   y_a->disable();
   y_b->disable();
   z_a->disable();
   z_b->disable();
}
LinearDriverMotherBoard::~LinearDriverMotherBoard()
{
   delete(z_a);
   delete(z_b); 
   delete(turnon_x);  
   delete(turnon_y);  
   delete(turnon_z);  
}
 LinearDriverMotherBoard::LinearDriverMotherBoard(ConfigLinearDriveNew configlineardrive)
 {
  if  (flgDebug)
  {
   afc.clear();
   afc =code+std::to_string(DEBUG)+ " MB Pico2040 ";
   afc += endln;//"\n";
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  }
    _configlineardrive=configlineardrive;
        z_a=new OutputPort(_configlineardrive.A_Port); 
        z_b=new OutputPort(_configlineardrive.B_Port); 
   turnon_x=new OutputPort(_configlineardrive.XTurn_on_Port); 
   turnon_y=new OutputPort(_configlineardrive.YTurn_on_Port); 
   turnon_z=new OutputPort(_configlineardrive.ZTurn_on_Port); 
   z_a->disable();
   z_b->disable();
   turnon_x->disable();
   turnon_y->disable();
   turnon_z->disable();
 }

void LinearDriverPico2040::activate(int command, int freq, int duty, int n, bool dir)  const ///
{
  OutputPort *ptrA = z_a;
  OutputPort *ptrB = z_b;
  if (command == 90)
  {
    ptrA = x_a;
    ptrB = x_b;
  }
  else
  if (command == 95)
  {
    ptrA = y_a;
    ptrB = y_b;
  } 
  else if (command == 99)
  {
    ptrA = z_a;
    ptrB = z_b;
  }
  double t_abs =(double)(1000000 / freq);        // 2000                     // mf 23108
  double t_low =(double)(duty * t_abs / 1000);  //  750 * 2000 / 1000000 = 1.5 // mf 23108
  double t_high = t_abs - t_low;    // 2 - 1.5 = 0.5

  if (dir)
  {
    std::swap(ptrA, ptrB);
  }

  ptrA->enable();
  ptrB->enable(); 
  sleep_ms(2);
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

void LinearDriverMotherBoard::activate(int command, int freq, int p, int n, bool dir) const ///
{
 /*
  if  (flgDebug)
  {
   afc.clear();
   afc =code+std::to_string(DEBUG)+ " MB Pico2040 active ";
   afc += +endln;//"\n";
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  }
  */
  OutputPort *ptrA = z_a; 
  OutputPort *ptrB = z_b;
  if (command == 90)//x
  {
   turnon_x->enable();
   turnon_y->disable();
   turnon_z->disable();
  }
  if (command == 95)//y
  {
   turnon_y->enable();
   turnon_x->disable();
   turnon_z->disable();
  }
  else
  if (command == 99)//z
  {
   turnon_z->enable();
   turnon_x->disable();
   turnon_y->disable(); 
  }
  double t_abs =(double)(1000000 / freq);        // 2000                     // mf 23108
  double t_low =(double)(p * t_abs / 1000);  //  750 * 2000 / 1000000 = 1.5 // mf 23108
  double t_high = t_abs - t_low;    // 2 - 1.5 = 0.5

  if (dir)
  {
    std::swap(ptrA, ptrB);
  }

  ptrA->enable();
  ptrB->enable(); 
  sleep_ms(2); 
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


