#include "scanner.hpp"
#include <iostream>
#include "../loop/common_data/common_variables.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "../utilities/peripheral_functions.hpp"


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
 // std::cout << conf_.delayB << ' ' << conf_.delayF << '\n';

  std::cout<<conf_.nPoints_x<<','<<conf_.nPoints_y<<'\n';

  if (!flgVirtual) move_to(point, 10);
  for (uint16_t i = 0; i < conf_.nPoints_y; i++) //231030
  {
    for (u_int16_t j = 0; j < conf_.nPoints_x; j++)
    {
      red();
      for (u_int16_t k = 0; k < conf_.betweenPoints_x; ++k)
      {
       if (!flgVirtual)  set_on_cap(1, ++pos_.x);
        sleep_us(conf_.delayF);
      }
      sleep_ms(50); // CONST 50ms
      if (!flgVirtual) 
      {
        getValuesFromAdc(); ///????? 231025
        getValuesFromAdc();
        vector_z.emplace_back(spiBuf[0]);  // get Z from adc ?? 0 or 1!!!!!
      }
      else
      {
        vector_z.emplace_back(j*100+i*100);  // get Z from adc
      } 
      if (conf_.flag != 0)
      {
        other_info.emplace_back(spiBuf[conf_.flag]);
      }
    }
    for (u_int16_t j = 0; j < conf_.betweenPoints_x * conf_.nPoints_x; ++j) // GET back
    {
      set_on_cap(1, --pos_.x);
      sleep_us(conf_.delayB);
    }
    afc.clear();
    afc="code50";
    for (size_t j = 0; j < vector_z.size(); j++)     // send info
    {
      afc+=','+std::to_string(vector_z[j] ); //231025
      if (conf_.flag != 1)  //231030
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
    if (STOP_ALL)                     // is need to stop
    {
   /*   blue();
      STOP_ALL = false;
  //    stop_scan();
      MICRO_SCAN = false;
      green(); 
   */
      break;
    //  return;
    }
    for (u_int16_t j = 0; j < conf_.betweenPoints_y; ++j) // go next line
    {
      set_on_cap(2, ++pos_.y);
      sleep_us(conf_.delayF);
    }
    blue();
  }//i
//  blue();
  STOP_ALL = false;
  MICRO_SCAN = false;
  if (!flgVirtual)  stop_scan();
  sleep_ms(200); //231030!!!
  std::cout<<"end\n"; //add end scan 231025
//  green();
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
