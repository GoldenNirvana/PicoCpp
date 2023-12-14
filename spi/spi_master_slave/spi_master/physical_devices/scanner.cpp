#include "scanner.hpp"
#include <iostream>
#include "../loop/common_data/common_variables.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "../utilities/peripheral_functions.hpp"
#include <cmath>
//#include <chrono>
//#include <ctime>    


Scanner::Scanner() : pos_({0, 0}), conf_({})
{}

Scanner::~Scanner()
{
  move_to({0, 0}, 10);
}

void Scanner::retract() //втянуть
{
  io3_1.enable();
}

void Scanner::protract() //вытянуть
{
  io3_1.disable();
}

bool Scanner::getHoppingFlg()
{
  return (bool)conf_.flgHoping;
}

void Scanner::start_scan()
{
  prev_point = pos_; //запоминание начальной точки скана
  vector_z.clear();
  other_info.clear();

  afc.clear();
  afc = "debug scan parameters";
  for (int j = 1; j <= 12; ++j)
  {
    afc += ',' + std::to_string(vector[j]);
  }
  afc += ',' + std::to_string(pos_.x) + ',' + std::to_string(pos_.y);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);

  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepslowline, stepfastline;
  uint8_t portx = 1;
  uint8_t porty = 2;
  uint8_t portfast;
  uint8_t portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;
  stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
  stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
  reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
  reststepy = conf_.betweenPoints_y % conf_.diskretinstep;

  afc.clear();
  afc = "debug scan parameters stepsxy  ";
  afc +=std::to_string(stepsx)+','+ std::to_string(stepsy)+','+ std::to_string(reststepx)+','+ std::to_string(reststepy);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);

  switch (conf_.path)
  {
    case 0://X+
    {
      portfast = portx;
      portslow = porty;
      pos_fast = pos_.x;
      pos_slow = pos_.y;
      nfastline = conf_.nPoints_x;
      nslowline = conf_.nPoints_y;
      stepslowline = stepsy;
      stepfastline = stepsx;
      reststepfast = reststepx;
      reststepslow = reststepy;
      break;
    }
    case 1: //Y+
    {
      portfast = porty;
      portslow = portx;
      pos_fast = pos_.y;
      pos_slow = pos_.x;
      nfastline = conf_.nPoints_y;
      nslowline = conf_.nPoints_x;
      stepslowline = stepsx;
      stepfastline = stepsy;
      reststepfast = reststepy;
      reststepslow = reststepx;
      break;
    }
  }
  for (uint32_t i = 0; i < nslowline; ++i)
  {
    stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
    stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
    reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
    reststepy = conf_.betweenPoints_y % conf_.diskretinstep;
    switch (conf_.path)
    {
      case 0://X+
      {
        stepslowline = stepsy;
        stepfastline = stepsx;
        reststepfast = reststepx;
        reststepslow = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepslowline = stepsx;
        stepfastline = stepsy;
        reststepfast = reststepy;
        reststepslow = reststepx;
        break;
      }
    }
    for (uint32_t j = 0; j < nfastline; ++j)
    {
      for (uint32_t k = 0; k < stepfastline; ++k) //move to next point
      {
        if (!flgVirtual)
        {
          set_on_dac(portfast, pos_fast);
          pos_fast += conf_.diskretinstep;
        } else
        { pos_fast += conf_.diskretinstep; }
        sleep_us(conf_.delayF);
      }
      if (reststepfast != 0)// добирание остатка
      {
        if (!flgVirtual)
        {
          pos_fast += reststepfast;
          set_on_dac(portfast, pos_fast);
        } else
        { pos_fast += reststepfast; }
        sleep_us(conf_.delayF);
      }

      //******************************************************************************
      // sleep_ms(conf_.pause); // 50 CONST 50ms wait for start get data
      sleep_us(conf_.pause);    // 50 CONST 50ms wait for start get data
      //*//******************************************************************************
      if (!flgVirtual)
      {
        getValuesFromAdc();
        getValuesFromAdc();
        vector_z.emplace_back((int16_t) spiBuf[0]);  // get Z from adc ??
        if (conf_.size == 2)
          switch (conf_.method)
            //added signal  Const  BackPass=2;    //PM  Const  Phase=3;  Const  UAM=4;   //Force Image
          {
            case 3:
            {
              other_info.emplace_back((int16_t) spiBuf[1]); //phase
              break;
            }
            case 4:
            {
              other_info.emplace_back((int16_t) spiBuf[1]); //ampl
              break;
            }
          }
      } else
      {
        vector_z.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));  // get Z from adc
        if (conf_.size == 2)  //added signal
        {
          other_info.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));
        }
      }
    }

    for (uint32_t l = 0; l < stepfastline * nfastline; ++l) // move  back
    {
      if (!flgVirtual)
      {
        pos_fast -= conf_.diskretinstep;
        set_on_dac(portfast, pos_fast);
      } else
      { pos_fast -= conf_.diskretinstep; }
      sleep_us(conf_.delayB);
    }

    if (reststepfast != 0)// добирание остатка
    {
      if (!flgVirtual)
      {
        pos_fast -= reststepfast;
        set_on_dac(portfast, pos_fast);
      } else
      { pos_fast -= reststepfast; }

      sleep_us(conf_.delayF);
    }
    afc.clear();
    afc = "code50";
    for (size_t m = 0; m < vector_z.size(); m++)     // send data scanline
    {
      switch (conf_.size)
      {
        case 1:
        {
          afc += ',' + std::to_string(vector_z[m]);
          break;
        }
        case 2: //z+ signal
        {
          afc += ',' + std::to_string(vector_z[m]) + ',' + std::to_string(other_info[m]);
          break;
        }
      }
    }
    afc += "\n";
    std::cout << afc;
    sleep_ms(300); //don't delete ! 100; 300 // 231130
    afc.clear();
    vector_z.clear();
    other_info.clear();
    if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE = false;
      conf_.delayF = vector[1];
      conf_.delayB = vector[2];
      set_io_value(2, vector[3]); //gain PID
      sleep_ms(100);              //
      conf_.diskretinstep = vector[4]; //????

      afc.clear();
      afc = "debug parameters update";
      for (int j = 1; j <= 3; ++j)
      {
        afc += ',' + std::to_string(vector[j]);
      }
      afc += +"\n";
      std::cout << afc;
      afc.clear();
      sleep_ms(100);
      //    dark();
    }
    if (!SCANNING)           // stop
    {
      SCANNING = false;
      sleep_ms(100);
      afc.clear();
      afc = "stopped\n";
      std::cout << afc;
      sleep_ms(100);
      break;
    }
    if ((nslowline - 1 - i) > 0)  //don't need for last line
    {
      if (conf_.method != 11) //not one line
      {
        for (uint32_t n = 0; n < stepslowline; ++n) // go next line
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            set_on_dac(portslow, pos_slow);
          } else
          { pos_slow += conf_.diskretinstep; }

          sleep_us(conf_.delayF);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) // last step
          {
            pos_slow -= reststepslow;
            set_on_dac(portslow, pos_slow);
          } else
          { pos_slow -= reststepslow; }

          sleep_us(conf_.delayF);
        }
      }
    }
  } //for

  blue();

  SCANNING = false;
  switch (conf_.path)
  {
    case 0:
    {
      pos_.x = pos_fast;
      pos_.y = pos_slow;
      break;
    }
    case 1:
    {
      pos_.x = pos_slow;
      pos_.y = pos_fast;
      break;
    }
  }
  stop_scan();  //возврат в начальную точку скана
  sleep_ms(300); //200
  red();
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )
  {
    sleep_ms(100);
    count++;
  } //ожидание ответа ПК для синхронизации
  TheadDone = false;
  green();
  std::cout << "end\n";
  activateDark();
}

void Scanner::start_hopingscan()
{
  prev_point = pos_; //запоминание начальной точки скана
  vector_z.clear();
  other_info.clear();

  afc.clear();
  afc = "debug scan parameters";
  for (int j = 1; j <= 15; ++j)
  {
    afc += ',' + std::to_string(vector[j]);
  }
  afc += ',' + std::to_string(pos_.x) + ',' + std::to_string(pos_.y);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);

  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepslowline, stepfastline;
  uint8_t portx = 1;
  uint8_t porty = 2;
  uint8_t portfast;
  uint8_t portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;
  stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
  stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
  reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
  reststepy = conf_.betweenPoints_y % conf_.diskretinstep;

  switch (conf_.path)
  {
    case 0://X+
    {
      portfast = portx;
      portslow = porty;
      pos_fast = pos_.x;
      pos_slow = pos_.y;
      nfastline = conf_.nPoints_x;
      nslowline = conf_.nPoints_y;
      stepslowline = stepsy;
      stepfastline = stepsx;
      reststepfast = reststepx;
      reststepslow = reststepy;
      break;
    }
    case 1: //Y+
    {
      portfast = porty;
      portslow = portx;
      pos_fast = pos_.y;
      pos_slow = pos_.x;
      nfastline = conf_.nPoints_y;
      nslowline = conf_.nPoints_x;
      stepslowline = stepsx;
      stepfastline = stepsy;
      reststepfast = reststepy;
      reststepslow = reststepx;
      break;
    }
  }
  for (uint32_t i = 0; i < nslowline; ++i)
  { 
   for (uint32_t j = 0; j < nfastline; ++j)
    {
      if (!flgVirtual)
      {
         retract();
      }   
      sleep_us(50);
      for (uint32_t k = 0; k < stepfastline; ++k) //move to next point
      {
        if (!flgVirtual)
        {
          set_on_dac(portfast, pos_fast);
          pos_fast += conf_.diskretinstep;
        } else
        { pos_fast += conf_.diskretinstep; }
        sleep_us(conf_.delayF);
      }
      if (reststepfast != 0)// добирание остатка
      {
        if (!flgVirtual)
        {
          pos_fast += reststepfast;
          set_on_dac(portfast, pos_fast);
        } else
        { pos_fast += reststepfast; }

        sleep_us(conf_.delayF);
      }

  //******************************************************************************
      if (!flgVirtual)
      {
          protract();
      }
      sleep_ms(conf_.HopeDelay);
      sleep_us(conf_.pause);    // 50 CONST 50ms wait for start get data
      //*//******************************************************************************
      if (!flgVirtual)
      {
        getValuesFromAdc();
        getValuesFromAdc();
        vector_z.emplace_back((int16_t) spiBuf[0]);     // get Z from adc ??
        switch (conf_.method)
          //added signal  Const  BackPass=2;    //PM  Const  Phase=3;  Const  UAM=4;   //Force Image
        {
          case 3:
          {
            other_info.emplace_back((int16_t) spiBuf[1]);
            break;
          }
          case 4:
          {
            other_info.emplace_back((int16_t) spiBuf[1]);
            break;
          }
        }
      } else
      {
        vector_z.emplace_back(
            int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));  // get Z from adc
        if (conf_.size == 2)                               // added signal
        {
          other_info.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));
        }
      }
    }
     if (!flgVirtual)
     {
         retract();
     } 
    sleep_us(50);

    if (!flgVirtual)
    {
      pos_fast -= conf_.diskretinstep * stepfastline * nfastline;
      set_on_dac(portfast, pos_fast);
    }
    else
    { pos_fast -= conf_.diskretinstep * stepfastline * nfastline; }
    sleep_us(conf_.delayB);

    if (reststepfast != 0)// добирание остатка
    {
      if (!flgVirtual)
      {
        pos_fast -= reststepfast;
        set_on_dac(portfast, pos_fast);
      } else
      { pos_fast -= reststepfast; }

      sleep_us(conf_.delayF);
    }

    afc.clear();
    afc = "code50";
    for (size_t m = 0; m < vector_z.size(); m++)     // send data scanline
    {
      switch (conf_.size)
      {
        case 1:
        {
          afc += ',' + std::to_string(vector_z[m]);
          break;
        }
        case 2: //z+ signal
        {
          afc += ',' + std::to_string(vector_z[m]) + ',' + std::to_string(other_info[m]);
          break;
        }
      }
    }
    afc += "\n";
    std::cout << afc;
    sleep_ms(200); //don't delete ! 100; 300 // 231130
    afc.clear();
    vector_z.clear();
    other_info.clear();
    if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE = false;
      conf_.delayF = vector[1];
      conf_.delayB = vector[2];
      set_io_value(2, vector[3]); //gain PID
      conf_.HopeDelay = vector[4];

      sleep_ms(100);    //
      afc.clear();
      afc = "debug parameters update";
      for (int j = 1; j <= 4; ++j)
      {
        afc += ',' + std::to_string(vector[j]);
      }
      afc += +"\n";
      std::cout << afc;
      afc.clear();
      sleep_ms(100);
      //    dark();
    }
    if (!SCANNING)           // stop
    {
      SCANNING = false;
      sleep_ms(100);
      afc.clear();
      afc = "stopped\n";
      std::cout << afc;
      sleep_ms(100);
      break;
    }
    // go next line
    if ((nslowline - 1 - i) > 0)  //don't need for last line
    {
      if (!flgVirtual)
      {
        pos_slow += conf_.diskretinstep * stepslowline;
        set_on_dac(portslow, pos_slow);
      } else
      { pos_slow += conf_.diskretinstep * stepslowline; }
      sleep_us(conf_.delayF);
      if (reststepslow != 0)
      {
        if (!flgVirtual) // last step
        {
          pos_slow -= reststepslow;
          set_on_dac(portslow, pos_slow);
        } else
        { pos_slow -= reststepslow; }
        sleep_us(conf_.delayF);
      }
    }
  } //for i

  blue();

  SCANNING = false;
  switch (conf_.path)
  {
    case 0:
    {
      pos_.x = pos_fast;
      pos_.y = pos_slow;
      break;
    }
    case 1:
    {
      pos_.x = pos_slow;
      pos_.y = pos_fast;
      break;
    }
  }
  stop_scan();  //возврат в начальную точку скана
  sleep_ms(200);
  if (!flgVirtual)
  {
     protract();
  }
  sleep_ms(1000);
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )
  {
    sleep_ms(100);
    count++;
  } //ожидание ответа ПК для синхронизации
  TheadDone = false;
  conf_.flgHoping=0;
  std::cout << "end\n";
  activateDark();
}

void Scanner::start_fastscan()
{
  prev_point = pos_; //запоминание начальной точки скана
  vector_z.clear();
  afc.clear();
  afc = "debug fastscan parameters";
  for (int j = 1; j <= 13; ++j)
  {
    afc += ',' + std::to_string(vector[j]);
  }
  afc += ',' + std::to_string(pos_.x) + ',' + std::to_string(pos_.y) + ',' + std::to_string(conf_.flgOneFrame);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);

  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepslowline, stepfastline;
  uint8_t  portx = 1;
  uint8_t  porty = 2;
  uint8_t  portfast;
  uint8_t  portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;
  stepsx    = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
  stepsy    = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
  reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
  reststepy = conf_.betweenPoints_y % conf_.diskretinstep;

  switch (conf_.path)
  {
    case 0://X+
    {
      portfast  = portx;
      portslow  = porty;
      pos_fast  = pos_.x;
      pos_slow  = pos_.y;
      nfastline = conf_.nPoints_x;
      nslowline = conf_.nPoints_y;
      stepslowline = stepsx;
      stepfastline = stepsy;
      reststepfast = reststepx;
      reststepslow = reststepy;
      break;
    }
    case 1: //Y+
    {
      portfast  = porty;
      portslow  = portx;
      pos_fast  = pos_.y;
      pos_slow  = pos_.x;
      nfastline = conf_.nPoints_y;
      nslowline = conf_.nPoints_x;
      stepslowline = stepsy;
      stepfastline = stepsx;
      reststepfast = reststepy;
      reststepslow = reststepx;
      break;
    }
  }

  //  auto start = std::chrono::system_clock::now();
  // Some computation here

  while (FASTSCANNING)
  {
    for (uint32_t i = 0; i < nslowline; ++i)
    {
      for (uint32_t j = 0; j < nfastline; ++j)
      {
        for (uint32_t k = 0; k < stepfastline; ++k) //move to next point
        {
          if (!flgVirtual)
          {
            set_on_dac(portfast, pos_fast);
            pos_fast += conf_.diskretinstep;
          } else
          { pos_fast += conf_.diskretinstep; }
          sleep_us(conf_.delayF);
        }
        if (reststepfast != 0)// добирание остатка
        {
          if (!flgVirtual)
          {
            pos_fast += reststepfast;
            set_on_dac(portfast, pos_fast);
          } else
          { pos_fast += reststepfast; }

          sleep_us(conf_.delayF);
        }
//******************************************************************************
        // sleep_ms(conf_.pause); // 50 CONST 50ms wait for start get data
        sleep_us(conf_.pause);    // 50 CONST 50ms wait for start get data
//******************************************************************************     

        if (!flgVirtual)
        {
          getValuesFromAdc();
          getValuesFromAdc();
          vector_z.emplace_back((int16_t) spiBuf[0]);  // get Z from adc ??
        }
        else
        {
          vector_z.emplace_back(
              int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));  // get Z from adc
        }
      } //j

      for (uint32_t j = 0; j < stepfastline * nfastline; ++j) // move  back
      {
        if (!flgVirtual)
        {
          pos_fast -= conf_.diskretinstep;
          set_on_dac(portfast, pos_fast);
        } 
        else
        { pos_fast -= conf_.diskretinstep; }
        sleep_us(conf_.delayB);
      }

      if (reststepfast != 0)// добирание остатка
      {
        if (!flgVirtual)
        {
          pos_fast -= reststepfast;
          set_on_dac(portfast, pos_fast);
        }
        else
        { pos_fast -= reststepfast; }

        sleep_us(conf_.delayF);
      }
      if ((nslowline - 1 - i) > 0)  //don't need for last line
      {
        for (uint32_t n = 0; n < stepslowline; ++n) // go next line
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            set_on_dac(portslow, pos_slow);
          } else
          { pos_slow += conf_.diskretinstep; }

          sleep_us(conf_.delayF);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) // last step
          {
            pos_slow -= reststepslow;
            set_on_dac(portslow, pos_slow);
          } else
          { pos_slow -= reststepslow; }

          sleep_us(conf_.delayF);
        }
      }
    } //i

    afc.clear();
    afc = "code56";
    for (size_t j = 0; j < vector_z.size(); j++)     // send data scanline
    {
      afc += ',' + std::to_string(vector_z[j]);
    }
    vector_z.clear();
    afc += "\n";
    std::cout << afc;
    sleep_ms(100);
    afc.clear();

    stop_scan();  //возврат в начальную точку скана

    if (conf_.flgOneFrame == 1) { FASTSCANNING = false; };
  }  //while true;

  blue();

  FASTSCANNING = false;
  switch (conf_.path)
  {
    case 0:
    {
      pos_.x = pos_fast;
      pos_.y = pos_slow;
      break;
    }
    case 1:
    {
      pos_.x = pos_slow;
      pos_.y = pos_fast;
      break;
    }
  }
  stop_scan();  //возврат в начальную точку скана
  sleep_ms(200);
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )
  {
    sleep_ms(100);
    count++;
  } //ожидание ответа ПК для синхронизации
  TheadDone = false;
  std::cout << "end\n";
  activateDark();
}

void Scanner::stop_scan()
{
  move_to(prev_point, 10);
}

void Scanner::update(const Config &config)
{
  conf_ = config;
}

Point Scanner::getX0Y0()
{
  sleep_ms(200);
  afc.clear();
  afc = "code18";
  afc += ',' + std::to_string(pos_.x) + ',' + std::to_string(pos_.y) + "\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
  return pos_;
}

void Scanner::move_toX0Y0(int x, int y,
                          int delay)  //переместиться в начальную точку  скана из начальной точке предыдущего скана
{
  Point pointX0Y0;
  pointX0Y0.x = (uint16_t) (x);
  pointX0Y0.y = (uint16_t) (y);
  delay = (uint16_t) (delay);
  sleep_ms(100);
  afc.clear();
  afc = "debug moveto parameters";
  afc += ',' + std::to_string(pointX0Y0.x) + ',' + std::to_string(pointX0Y0.y) + ',' + std::to_string(delay);
  afc += ',' + std::to_string(pos_.x) + ',' + std::to_string(pos_.y);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(200);

  move_to(pointX0Y0, delay);
  sleep_ms(200);
  afc.clear();
  afc = "stopped\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(200);
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )
  {
    sleep_ms(100);
    count++;
  } //ожидание ответа ПК для синхронизации
  TheadDone = false;
  std::cout << "end\n";
}

void Scanner::move_to(const Point &point, uint16_t delay)
{
  if (!flgVirtual)
  {
    while (pos_.x < point.x)
    {
      set_on_dac(1, ++pos_.x);
      sleep_us(delay);
    }
    while (pos_.x > point.x)
    {
      set_on_dac(1, --pos_.x);
      sleep_us(delay);
    }
    while (pos_.y < point.y)
    {
      set_on_dac(2, ++pos_.y);
      sleep_us(delay);
    }
    while (pos_.y > point.y)
    {
      set_on_dac(2, --pos_.y);
      sleep_us(delay);
    }
  } else
  {
    pos_.x = point.x;
    pos_.y = point.y;
    sleep_us(delay * (std::abs(pos_.x - point.x) + std::abs(pos_.y - point.y)));
  }
}

void Scanner::move_toZ0(int lid_name, int f, int p, int n, int dir)  //отвестись в безопастную начальную точку по Z
{
 if (!flgVirtual)
 {
  scanner.retract();  //втянуть сканер
  sleep_ms(50);
  if (!flgVirtual) linearDriver.activate(lid_name, f, p, std::abs(n), dir);
  scanner.protract();  //вытянуть сканер
 } 
  sleep_ms(1000);
  afc.clear();
  afc = "debug autorising done " + std::to_string(n) + ',' + std::to_string(dir);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
}

//void Scanner::positioningXYZ(int lid_name, int f, int p, int n, int dir, int16_t gtmax, int16_t gtmin) // n-nsteps
void Scanner::positioningXYZ(const int16_t *const data)
{
  //uint32_t SET_POINT;
  uint8_t lid_name;
  uint16_t GATE_Z_MAX, GATE_Z_MIN;
  int8_t status;
  const int none = 30;
  const int ok = 3;
  const int touch = 2;
  int16_t ln;  ///????? sign abs?
  bool ldir;
  int16_t p,f;
 // SET VALUE FROM RX_CORE
      lid_name=(uint8_t)data[0]; //  int lid_name
      f=data[1]; //  int f
      p=data[2]; //  int p
      ln=data[3]; //  int n
      ldir=(bool)data[4]; //  int dir
      GATE_Z_MAX=data[5]; //  int Z gate max
      GATE_Z_MIN=data[6]; //  int Z gate min
   //   pos_data[7] / //  0= SFM, 1=STM ;SICMAC-2; SICMDC-3;  device type
  //    pos_data[8]/ //  Voltage
    
  red();

  if (lid_name == 90 || lid_name == 95) //X,Y
  {
    while (LID_UNTIL_STOP)
    {
      if (POSXYZ_CONFIG_UPDATE)
      {
        POSXYZ_CONFIG_UPDATE = false;
        ln = vector[1]; // with sign
        GATE_Z_MAX = vector[2];
        GATE_Z_MIN = vector[3];
        ldir = 0;
        if (ln > 0) ldir = 1;
        ln = abs(ln);
        sleep_ms(100);
        afc.clear();
        afc = "debug parameters update";
        for (int j = 1; j <= 3; ++j)
        {
          afc += ',' + std::to_string(vector[j]);
        }
        afc += +"\n";
        std::cout << afc;
        afc.clear();
        sleep_ms(100);

      }
      status = none;
      if (!flgVirtual) //add mf
      {
        linearDriver.activate(lid_name, f, p, std::abs(ln), ldir);
      } else
      {

      }
      afc.clear(); //231025
      afc = "code" + std::to_string(lid_name) + ',' + std::to_string(status) + ',' + std::to_string(ZValue) +
            ',' +
            std::to_string(SignalValue) // Zval,SigVal??
            //+','+std::to_string(ln)
            + "\n";
      std::cout << afc;
      sleep_ms(100); //231104
      afc.clear();
    }
  }
  if (lid_name == 99) //Z
  {
    status = none;
    while (LID_UNTIL_STOP)
    {
      Z_STATE = true;
      if (POSXYZ_CONFIG_UPDATE)
      {
        ln = vector[1];
        GATE_Z_MAX = vector[2];
        GATE_Z_MIN = vector[3];
        ldir = 0;
        if (ln > 0) ldir = 1;
        POSXYZ_CONFIG_UPDATE = false;
        sleep_ms(100);
        afc.clear();
        afc = "debug parameters update";
        for (int j = 1; j <= 3; ++j)
        {
          afc += ',' + std::to_string(vector[j]);
        }
        afc += +"\n";
        std::cout << afc;
        afc.clear();
        sleep_ms(100);
      }
      status = none;
      if (!flgVirtual) //add mf
      {
        getValuesFromAdc();
        auto ptr = getValuesFromAdc();
        ZValue      = (int16_t) ptr[ZPin];
        SignalValue = (int16_t) ptr[AmplPin];
        // check if z > <
        if (ldir == 1)
        {
          if (ZValue < GATE_Z_MIN)
          {
            status = touch;
            break;
          }
          if (ZValue < GATE_Z_MAX)
          {
            status = ok;
            break;
          }
        }
        linearDriver.activate(lid_name, f, p, std::abs(ln), ldir);
      } 
      else //virtual
      {
        if (ldir == 1)
        { ZValue -= ln; }
        else
        {
          if (ZValue < (ZMaxValue - ln)) { ZValue += ln;       }
          else                           { ZValue = ZMaxValue; }
        }
        // check if z > <
        if (ZValue < GATE_Z_MIN)
        {
          status = touch;
          break;
        }
        if (ZValue < GATE_Z_MAX)
        {
          status = ok;
          break;
        }
        sleep_ms(100); //need to adjust       
      }
      afc.clear();
      afc = "code" + std::to_string(lid_name) + ',' + std::to_string(status) + ',' + std::to_string(ZValue) +
            ',' + std::to_string(SignalValue) + "\n";
      std::cout << afc;
      sleep_ms(200); //need to adjust
      afc.clear();
    }
  }
  afc.clear();
  afc = "code" + std::to_string(lid_name) + ',' + std::to_string(status) + ',' + std::to_string(ZValue) + ',' +
        std::to_string(SignalValue)
        // +','+std::to_string(GATE_Z_MIN)+','+std::to_string(GATE_Z_MAX)
        + "\n";
  std::cout << afc;
  sleep_ms(200); //need to adjust
  afc.clear();
  afc = "stopped\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )
  {
    sleep_ms(100);
    count++;
  } //ожидание ответа ПК для синхронизации
  TheadDone = false;
  LID_UNTIL_STOP = false;
  std::cout << "end\n";
  dark();
  //  sleep_ms(100); //need to adjust
}
void Scanner::spectroscopyIV(int32_t vector[16])
{

}
void Scanner::approacphm(const int16_t *const data) //uint16_t
{
  const int none = 30;
  const int ok = 3;
  const int touch = 2;
  const int stopdone = 1;

  uint16_t ZMaxValue = 32767;
  uint16_t SET_POINT, GATE_Z_MAX, GATE_Z_MIN;
  int16_t freq, scv;//
  int16_t GAIN, NSTEPS;
  uint16_t INTDELAY, SCANNERDECAY;
  uint8_t flgDev;
  int16_t Bias;
  // SET VALUE FROM RX_CORE
  SET_POINT      = data[0];
  GATE_Z_MAX     = data[1];
  GATE_Z_MIN     = data[2];
  NSTEPS         = data[3];
  INTDELAY       = data[4];
  GAIN           = data[5];
  SCANNERDECAY   = data[6]; 
  freq           = data[7];
  scv            = data[8];
  flgDev= (uint8_t)data[9]; // 0= SFM, 1=STM ;SICMAC-2; SICMDC-3;  device type
  Bias           = data[10];         //Voltage for 1,2,3
 //need to add channel Bias ????
 //need to add channel SetPoint ????

  afc.clear();
  afc = "debug approach parameters ";
  for (size_t j = 0; j < 9; j++)     // send info
  {
    afc += ',' + std::to_string(data[j]);
  }
  afc += std::to_string(AmplPin) + ',' + std::to_string(ZPin) + "\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(200);

 // dac8563_1.writeA(SET_POINT); ///??????
 set_SetPoint(0,SET_POINT); //add 231214
  if (flgDev!=0) set_Bias(1,Bias);  //add 231214???
  set_gainPID(GAIN);
  
  std::vector<int16_t> buf_params;
  buf_params.reserve(7);

  for (int i = 0; i < 7; ++i)  buf_params.push_back(data[i]);

  if (!flgVirtual) //add mf
  {
    getValuesFromAdc();
    uint16_t *ptr = getValuesFromAdc(); 
    ZValue = (int16_t) ptr[ZPin];
    switch (flgDev)
   {
     case 0: 
     {
      SignalValue = (int16_t) ptr[AmplPin];
      break;  
     } 
   case 1:
   case 3:  
     {
      SignalValue = (int16_t) ptr[IPin];
      break;  
     } 
   }
  }
  std::vector<int16_t> buf_status;
  buf_status.push_back(none);
  buf_status.push_back(ZValue);
  buf_status.push_back(SignalValue);
  afc.clear();
  afc =
      "code75," + std::to_string(APPROACH) + ',' + std::to_string(buf_status[1]) + ',' +
      std::to_string(buf_status[2]) +
      "\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);

  while (true)
  {
    buf_status[0] = none;
    if (!APPROACH)
    {
      buf_status[0] = stopdone;
      buf_status[1] = ZValue;
      buf_status[2] = SignalValue;
      sleep_ms(100);  /// need for virtual для разделение afc
      afc.clear();
      afc = "stopped\n";
      std::cout << afc;
      afc.clear();
      sleep_ms(100);
      break;
    }
    if (APPROACH_CONFIG_UPDATE)
    {
      // log("config updated\n",flgDebugLevel);
      APPROACH_CONFIG_UPDATE = false;
      SET_POINT = vector[1];
      GATE_Z_MAX = vector[2];
      GATE_Z_MIN = vector[3];
      NSTEPS = vector[4];
      INTDELAY = vector[5];
      GAIN = vector[6];
      SCANNERDECAY = vector[7];
     
      if (flgDev!=0) set_Bias(1,Bias);  

      dac8563_1.writeA(SET_POINT);
   // set_SetPoint(0,SET_Point); //add 231214 ?
   // set_gainPID(GAIN);
      set_io_value(2, GAIN); 
      sleep_ms(100);  // need for virtual для разделение afc
      afc.clear();
      afc = "debug parameters update";
      for (int j = 1; j <= 7; ++j)
      {
        afc += ',' + std::to_string(vector[j]);
      }
      afc += +"\n";
      std::cout << afc;
      afc.clear();
      sleep_ms(100);
  
    }
  
    sleep_ms(INTDELAY);

    if (!flgVirtual)
    {
      getValuesFromAdc();
      auto ptr = getValuesFromAdc();
     switch (flgDev)
    {
     case 0: 
     {
      SignalValue = (int16_t) ptr[AmplPin];
      break;  
     } 
     case 1:
     case 3:  
     {
      SignalValue = (int16_t) ptr[IPin];
      break;  
     } 
    }     
    if (flgDebugLevel <= DEBUG_LEVEL);//log("Z = " + std::to_string(Z) + '\n',flgDebugLevel);
    }
    else
    {
      if (NSTEPS >= 0)
      {
        ZValue = ZValue - 500;
      } else
      {
        if ((ZMaxValue - ZValue) > 0)
        { ZValue += 500; }
        else
        { ZValue = ZMaxValue; }
      }
      buf_status[1] = ZValue;
      buf_status[2] = SignalValue;
    }
    if (NSTEPS >= 0)
    {
      if (ZValue <= GATE_Z_MIN)
      {
        buf_status[0] = touch;
        buf_status[1] = ZValue;
        buf_status[2] = SignalValue;
        break;
      }
      if (ZValue <= GATE_Z_MAX)
      {
        int k = 0;
        for (int i = 0; i < 10; ++i)
        {
          if (!flgVirtual)
          {
           getValuesFromAdc();
            auto ptr = getValuesFromAdc();
            ZValue = (int16_t) ptr[ZPin];
          }
          if (ZValue <= GATE_Z_MAX) k++;
          if (k == 3)
          {
            buf_status[0] = ok;
            buf_status[1] = ZValue;
            buf_status[2] = SignalValue;
            if (flgDebugLevel <= DEBUG_LEVEL);// log("success\n",flgDebugLevel);
            break;
          }
          sleep_ms(10);
        }
        if (buf_status[0] == 3)
        { break; }
      }
    } //NSTEPS>0
    if (NSTEPS < 0)
    {

    }
    afc.clear();
    afc = "code75," + std::to_string(buf_status[0]) + ',' + std::to_string(buf_status[1]) + ',' +
          std::to_string(buf_status[2]) + "\n";
    std::cout << afc;
    afc.clear();
    if (!flgVirtual)
    {
      retract(); //io3_1.enable(); // втянуть 231025
      sleep_ms(SCANNERDECAY);
      linearDriver.activate(99, freq, scv, std::abs(NSTEPS), NSTEPS > 0);
      protract(); //io3_1.disable(); //вытянуть
    }
  }
  afc.clear();
  afc = "code75," + std::to_string(buf_status[0]) + ',' + std::to_string(buf_status[1]) + ',' +
        std::to_string(buf_status[2]) + "\n";
  std::cout << afc;
  afc.clear();
  APPROACH = false;
  if (!flgVirtual)
  {
    protract();// io3_1.disable();//вытянуть
    sleep_ms(INTDELAY);
  }
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )
  {
    sleep_ms(100);
    count++;
  } //ожидание ответа ПК для синхронизации
  TheadDone = false;
  std::cout << "end\n";
}

void Scanner::start_frqscan()
{
  static uint16_t inBuf[5]; // n, start_freq, step, channel, delay
  int16_t signalvalue;
  int16_t res_freq = 10000;
  int16_t a = 10000;
  int16_t scan_index = 0;
  int16_t current_freq = 0;
  afc.clear();
  afc = "debug frq scan parameters ";
  for (int j = 0; j < 5; ++j)
  {
    inBuf[j] = vector[1 + j];
    afc += ',' + std::to_string(inBuf[j]);
  }
  afc += ',' + std::to_string(flgVirtual) + std::to_string(AmplPin) + ',' + std::to_string(ZPin) + "\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);

  current_channel = inBuf[3] - 1;  //ampl=0;
  afc = "code25";
  while ((scan_index++ < inBuf[0]))
  {
    if (!flgVirtual)
    {
      set_freq(inBuf[1]);
      sleep_ms(inBuf[4]);
      getValuesFromAdc();
      afc +=
          ',' + std::to_string(inBuf[1]) + ',' +
          std::to_string((int16_t) getValuesFromAdc()[current_channel]);//+ ',';
    } else
    {
      current_freq = inBuf[1];
      sleep_ms(inBuf[4]);
      signalvalue = (int16_t) std::round(a * (pow(M_E, -pow((current_freq - res_freq), 2) / 1000000))); //231126
      afc += ',' + std::to_string(current_freq) + ',' + std::to_string(signalvalue);//+',';
    }
    sleep_ms(10);
    inBuf[1] += inBuf[2];
  }
  std::cout << afc << '\n';
  sleep_ms(100);
  afc.clear();
  current_channel = -1;
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )
  {
    sleep_ms(100);
    count++;
  } //ожидание ответа ПК для синхронизации
  TheadDone = false;
  std::cout << "end\n";
  RESONANCE = false;
  RESONANCE_STOP = false;
}

