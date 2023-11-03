#include "scanner.hpp"
#include <iostream>
#include "../loop/common_data/common_variables.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "../utilities/peripheral_functions.hpp"
#define _USE_MATH_DEFINES // for C++
#include <cmath>

#define PI 3.14159265 

Scanner::Scanner() : pos_({0, 0}), conf_({})
{}

Scanner::~Scanner()
{
  move_to({0, 0}, 10);
}

void Scanner::start_scan(const Point &point)
{
  prev_point = pos_;
  vector_z.clear(); //231030;
  other_info.clear();
 
  if (!flgVirtual) move_to(point, 10);
  for (uint32_t i = 0; i < conf_.nPoints_y; ++i) //231030
  {
    for (uint32_t j = 0; j < conf_.nPoints_x; ++j)
    {
      for (uint16_t k = 0; k < conf_.betweenPoints_x; ++k)
      {
       if (!flgVirtual)  set_on_cap(1, ++pos_.x);
        sleep_us(conf_.delayF);
      }
      sleep_ms(50); // CONST 50ms
      if (!flgVirtual) 
      {
        getValuesFromAdc(); ///????? 231025
        getValuesFromAdc();
        vector_z.emplace_back(spiBuf[0]);  // get Z from adc ?? 
        if (conf_.flag == 2) //231102
        {
         other_info.emplace_back(spiBuf[conf_.flag]); //????
        }
      }
      else
      {
        vector_z.emplace_back(uint16_t(10000.0*(sin(M_PI*j*0.1)+sin(M_PI*i*0.1))));  // get Z from adc
        if (conf_.flag == 2) //231102
        {
         other_info.emplace_back(uint16_t(10000.0*(sin(M_PI*j*0.1)+sin(M_PI*i*0.1)))); //????
        }
      }   
    }
    for (uint16_t j = 0; j < conf_.betweenPoints_x * conf_.nPoints_x; ++j) // GET back
    {
      set_on_cap(1, --pos_.x);
      sleep_us(conf_.delayB);
    }
    afc.clear();
    afc="code50";
    for (size_t j = 0; j < vector_z.size(); j++)     // send info
    {
      afc+=','+std::to_string(vector_z[j] ); //231025
      if (conf_.flag == 2)  //231030
      {
        afc+=','+std::to_string(vector_z[j] )+','+std::to_string(other_info[j]); //231025
      };    
    }
      afc+="\n";
      std::cout<<afc;
      afc.clear();
      vector_z.clear();
      other_info.clear();
  //  std::cout << "END_LINE\n"; //edited 231025
    if (CONFIG_UPDATE) //add mf 231101
    {
        CONFIG_UPDATE = false;
        red();
        conf_.delayF=vector[1];
        conf_.delayB=vector[2];
        set_io_value(2,vector[3]);
        dark();
    }
    if (STOP_ALL)                     // is need to stop
    {
   /*   blue();
      STOP_ALL = false;
  //    stop_scan(); //????  231102
      MICRO_SCAN = false;
      green(); 
   */
      break;
    //  return; ///??? 231102
    }
    for (uint16_t j = 0; j < conf_.betweenPoints_y; ++j) // go next line
    {
      set_on_cap(2, ++pos_.y);
      sleep_us(conf_.delayF);
    }
    activateBlue();
    sleep_ms(100);
    activateDark();
  }//i
//  blue();
  STOP_ALL = false;
  MICRO_SCAN = false;
  if (!flgVirtual)  stop_scan();
  sleep_ms(200); //231030!!!
  std::cout<<"end\n"; //add end scan 231025
  activateDark();
// green();
}

void Scanner::start_scan()
{
  start_scan(pos_);
}

void Scanner::stop_scan()
{
  move_to(prev_point, 10);
}

void Scanner::update(const Config &config)
{
  conf_ = config;
}

void Scanner::move_to(const Point &point, uint32_t delay)
{

  while (pos_.x < point.x)
  {
    set_on_cap(1, ++pos_.x);
    sleep_us(delay);
  }
  while (pos_.x > point.x)
  {
    set_on_cap(1, --pos_.x);
    sleep_us(delay);
  }
  while (pos_.y < point.y)
  {
    set_on_cap(2, ++pos_.y);
    sleep_us(delay);
  }
  while (pos_.y > point.y)
  {
    set_on_cap(2, --pos_.y);
    sleep_us(delay);
  }
  std::cout << "end\n";
}
