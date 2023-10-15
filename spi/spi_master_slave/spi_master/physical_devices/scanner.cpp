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
  red();
  prev_point = pos_;
  std::cout << conf_.delayB << ' ' << conf_.delayF << '\n';
  move_to(point, 10);
  for (int i = 0; i < conf_.nPoints_y; ++i)
  {
    for (int j = 0; j < conf_.nPoints_x; ++j)
    {
      for (int k = 0; k < conf_.betweenPoints_x; ++k)
      {
        set_on_cap(1, ++pos_.x);
        sleep_us(conf_.delayF);
      }
      sleep_ms(50); // CONST 50ms
      getValuesFromAdc();
      getValuesFromAdc();
      vector_z.emplace_back(spiBuf[0]);  // get Z from adc
      if (conf_.flag != 0)
      {
        other_info.emplace_back(spiBuf[conf_.flag]);
      }
    }
    for (int j = 0; j < conf_.betweenPoints_x * conf_.nPoints_x; ++j) // GET back
    {
      set_on_cap(1, --pos_.x);
      sleep_us(conf_.delayB);
    }
    for (int j = 0; j < vector_z.size(); j++)     // send info
    {
      std::cout << vector_z[j] << ' ';
      if (conf_.flag != 0)
      {
        std::cout << other_info[j] << ';';
      }
    }
    vector_z.clear();
    other_info.clear();
    std::cout << "END_LINE\n";
    if (STOP_ALL)                     // is need to stop
    {
      blue();
      STOP_ALL = false;
      stop_scan();
      MICRO_SCAN = false;
      green();
      return;
    }
    for (int j = 0; j < conf_.betweenPoints_y; ++j) // go next line
    {
      set_on_cap(2, ++pos_.y);
      sleep_us(conf_.delayF);
    }
  }
  blue();
  STOP_ALL = false;
  stop_scan();
  MICRO_SCAN = false;
  green();
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
