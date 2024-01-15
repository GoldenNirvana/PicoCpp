#include "scanner.hpp"
#include <iostream>
#include "../loop/common_data/common_variables.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "../utilities/peripheral_functions.hpp"
#include <cmath>

Scanner::Scanner() : pos_({0, 0}), conf_({})
{}

Scanner::~Scanner()
{
  move_to({0, 0}, 10);
}

void Scanner::sendStrdata(std::string const& header,const std::vector<int32_t> &data)
{
  std::string afcc;
  afcc.clear();
  afcc =header; 
   //for (auto & element :data) 
  for (size_t j = 0; j < data.size(); ++j)
  {
 // afc +=',' + std::to_string(element);
   afcc +=',' + std::to_string(data[j]);
  }
  afcc +="\n";
  std::cout << afcc;
  afcc.clear();
  sleep_ms(100);
}
void Scanner::sendStrdata(std::string const& header,const std::vector<uint16_t> &data)
{
  std::string afcc;
  afcc.clear();
  afcc =header;
   //for (auto & element :data) 
  for (size_t j = 0; j < data.size(); ++j)
  {
 // afc +=',' + std::to_string(element);
   afcc +=',' + std::to_string(data[j]);
  }
  afcc +="\n";
  std::cout << afcc;
  afcc.clear();
  sleep_ms(100);
}
void Scanner::sendStrdata(std::string const& header,int32_t *data, int16_t size)
{
 std::string afcc;
  afcc.clear();
  afcc =header;
   //for (auto & element :data) 
  for (int j = 0; j < size; ++j)
  {
 // afc +=',' + std::to_string(element);
   afcc +=',' + std::to_string(data[j]);
  }
  afcc +="\n";
  std::cout << afcc;
  afcc.clear();
  sleep_ms(100);
}


void Scanner::readADC()
{
  afc.clear();
  afc = "code12";
  if (!flgVirtual)
  {
   auto ptr = getValuesFromAdc();
  //logger(ptr, 8);
   ZValue = (int16_t) ptr[ZPin];
      switch (vector[1]) //прибор
   {
          case 0: //SFM
                {
                 SignalValue = (int16_t) ptr[AmplPin];
                 break;  
                } 
          case 1://STM
          case 3://SICMDC  
                {
                 SignalValue = (int16_t) ptr[IPin];
                 break;  
                } 
   }       
          afc +=',' + std::to_string(ZValue) + ',' + std::to_string(SignalValue) + ',' + std::to_string(vector[1]) + "\n";
          std::cout << afc;
          afc.clear();
  } 
  else
  {
          afc += ',' + std::to_string(ZValue) + ',' + std::to_string(SignalValue) +',' + std::to_string(vector[1])+"\n";   //Z,Signal
          std::cout << afc;
          afc.clear();
          sleep_ms(100);
  }
}
void Scanner::scanner_retract_protract(int port, int flg)
 {
  flg == 1 ? io_ports[port - 1].enable() : io_ports[port - 1].disable(); 
 } 

void Scanner::retract() //втянуть
{
  io3_1.enable();
}

void Scanner::protract() //вытянуть
{
  io3_1.disable();
}
void Scanner::LOOP_freeze_unfreeze(int port, int flg) // port virtual
{
 flg == 1 ? io_ports[port - 1].enable() : io_ports[port - 1].disable();
}
void Scanner::freezeLOOP()    // заморозить ПИД
{
  io3_0.enable(); //???
}

void Scanner::unfreezeLOOP()  // разморозить ПИД
{
  io3_0.disable();  //??
}

bool Scanner::getHoppingFlg() //получить флаг установлен ли флаг сканирования прыжками
{
  return (bool)conf_.flgHoping;
}

void Scanner::start_scan(int32_t *vector) //сканирование
{
  const int8_t oneline=11;
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
  uint8_t  portx = 1;
  uint8_t  porty = 2;
  uint8_t  portfast;
  uint8_t  portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;

 scan_update({
              static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]),
              static_cast<uint8_t>(vector[3]),  static_cast<uint8_t>(vector[4]),
              static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
              static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
              static_cast<uint8_t>(vector[9]),  static_cast<uint8_t>(vector[10]),
              static_cast<uint16_t>(vector[11]),static_cast<uint16_t>(vector[12]),
              static_cast<uint8_t>(vector[13]), static_cast<uint8_t>(vector[14]),
              static_cast<uint16_t>(vector[15])
             });

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
      for (uint32_t k = 0; k < stepfastline; ++k)  
      {
        if (!flgVirtual)
        {
          set_DACXY(portfast, pos_fast);
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
          set_DACXY(portfast, pos_fast);
        } else
        { pos_fast += reststepfast; }
        sleep_us(conf_.delayF);
      }

      //******************************************************************************
      sleep_us(conf_.pause);    // 50 CONST 50ms wait for start get data
      //*******************************************************************************
      if (!flgVirtual)
      {
        getValuesFromAdc();
        vector_z.emplace_back((int16_t) spiBuf[ZPin]);  // get Z from adc
        if (conf_.size == 2)
          switch (conf_.method)
          {
            case 3://phase 
            {
              other_info.emplace_back((int16_t) spiBuf[1]); 
              break;
            }
            case 4://ampl
            {
              other_info.emplace_back((int16_t) spiBuf[AmplPin]); 
              break;
            }
            case 7://current
            {
              other_info.emplace_back((int16_t) spiBuf[IPin]); 
              break;
            }
          }
      } else
      {
       if (conf_.method!=oneline) vector_z.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));  // get Z from adc
       else  vector_z.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * 0 * 0.1)))); 
        if (conf_.size == 2)  //дополнительный сигнал
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
        set_DACXY(portfast, pos_fast);
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
        set_DACXY(portfast, pos_fast);
      }
      else
      { pos_fast -= reststepfast; }

      sleep_us(conf_.delayF);
    }
  
  /*  afc.clear();
    afc = "code50";
    for (size_t m = 0; m < vector_z.size(); m++)     // подготовка результатов
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
    std::cout << afc;   // посылка результатов на ПК
    sleep_ms(300); //don't delete ! 100; 300 // 231130
    afc.clear();
    */
    sendStrdata("code50",vector_z);
    vector_z.clear();
    other_info.clear();
    if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE = false;
      conf_.delayF = vector[1];
      conf_.delayB = vector[2];
      set_gainPID(vector[3]);
      sleep_ms(100);              
      conf_.diskretinstep = vector[4]; 

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
    if (STOP)   // stop
    {
      STOP = false;
      sleep_ms(100);
      afc.clear();
      afc = "stopped\n";
      std::cout << afc;
      sleep_ms(100);
      break;
    }
    if ((nslowline - 1 - i) > 0)  //если непоследняя линия
    {
      if (conf_.method != oneline) //не сканирование по одной линии
      {
        for (uint32_t n = 0; n < stepslowline; ++n) // переход на следующую линию
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            set_DACXY(portslow, pos_slow);
          } else
          { pos_slow += conf_.diskretinstep; }

          sleep_us(conf_.delayF);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow -= reststepslow;
            set_DACXY(portslow, pos_slow);
          } else
          { pos_slow -= reststepslow; }

          sleep_us(conf_.delayF);
        }
      }
    }
  } 

  blue();

  //SCANNING = false;
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
  //red();
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

void Scanner::start_hopingscan(int32_t *vector)
{
  const int8_t oneline=11;
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
   scan_update({
               static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]),
               static_cast<uint8_t>(vector[3]),  static_cast<uint8_t>(vector[4]),
               static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
               static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
               static_cast<uint8_t>(vector[9]),  static_cast<uint8_t>(vector[10]),
               static_cast<uint16_t>(vector[11]),static_cast<uint16_t>(vector[12]),
               static_cast<uint8_t>(vector[13]), static_cast<uint8_t>(vector[14]),
               static_cast<uint16_t>(vector[15])
              });

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
      for (uint32_t k = 0; k < stepfastline; ++k) 
      {
        if (!flgVirtual)
        {
          set_DACXY(portfast, pos_fast);
          pos_fast += conf_.diskretinstep;
        } else
        { pos_fast += conf_.diskretinstep; }
        sleep_us(conf_.delayF);
      }
      if (reststepfast != 0)
      {
        if (!flgVirtual)
        {
          pos_fast += reststepfast;
          set_DACXY(portfast, pos_fast);
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
    //*******************************************************************************
      if (!flgVirtual)
      {
         getValuesFromAdc();
        vector_z.emplace_back((int16_t) spiBuf[ZPin]);     // считать  Z 
        switch (conf_.method)
          //added signal  Const  BackPass=2;    //PM  Const  Phase=3;  Const  UAM=4;   //Force Image
        {
          case 3://phase !!!!
          {
            other_info.emplace_back((int16_t) spiBuf[1]);
            break;
          }
          case 4://ampl
          {
            other_info.emplace_back((int16_t) spiBuf[AmplPin]);
            break;
          }
          case 7://current
          {
            other_info.emplace_back((int16_t) spiBuf[IPin]);
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
      set_DACXY(portfast, pos_fast);
    }
    else
    { pos_fast -= conf_.diskretinstep * stepfastline * nfastline; }
    sleep_us(conf_.delayB);

    if (reststepfast != 0)
    {
      if (!flgVirtual)
      {
        pos_fast -= reststepfast;
        set_DACXY(portfast, pos_fast);
      } else
      { pos_fast -= reststepfast; }

      sleep_us(conf_.delayF);
    }

    afc.clear();
    afc = "code50";
    for (size_t m = 0; m < vector_z.size(); m++)     
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
    std::cout << afc; //посылка данных cканирования на ПК
    sleep_ms(200); //don't delete ! 100; 300 // 231130
    afc.clear();
    vector_z.clear();
    other_info.clear();
    if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE = false;
      conf_.delayF = vector[1];
      conf_.delayB = vector[2];
      set_gainPID(vector[3]);
      conf_.HopeDelay = vector[4];
      sleep_ms(100);   
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
    }
    if (STOP)  // stop
    {
      STOP = false;
      sleep_ms(100);
      afc.clear();
      afc = "stopped\n";
      std::cout << afc;
      sleep_ms(100);
      break;
    }
     if ((nslowline - 1 - i) > 0)  //если непоследняя линия
    {
     if (conf_.method !=oneline) //не сканирование по одной линии
     {  if (!flgVirtual)
      {
        pos_slow += conf_.diskretinstep * stepslowline;
        set_DACXY(portslow, pos_slow);
      } else
      { pos_slow += conf_.diskretinstep * stepslowline; }
      sleep_us(conf_.delayF);
      if (reststepslow != 0)
      {
        if (!flgVirtual) 
        {
          pos_slow -= reststepslow;
          set_DACXY(portslow, pos_slow);
        } else
        { pos_slow -= reststepslow; }
        sleep_us(conf_.delayF);
      }
     } 
    }
  } 
  blue();
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

void Scanner::start_fastscan(int32_t *vector)
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

  scan_update({
               static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]),
               static_cast<uint8_t>(vector[3]),  static_cast<uint8_t>(vector[4]),
               static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
               static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
               static_cast<uint8_t>(vector[9]),  static_cast<uint8_t>(vector[10]),
               static_cast<uint16_t>(vector[11]),static_cast<uint16_t>(vector[12]),
               static_cast<uint8_t>(vector[13])
              }); 
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
  while (!STOP)
  {
    for (uint32_t i = 0; i < nslowline; ++i)
    {
      for (uint32_t j = 0; j < nfastline; ++j)
      {
        for (uint32_t k = 0; k < stepfastline; ++k) 
        {
          if (!flgVirtual)
          {
            set_DACXY(portfast, pos_fast);
            pos_fast += conf_.diskretinstep;
          } else
          { pos_fast += conf_.diskretinstep; }
          sleep_us(conf_.delayF);
        }
        if (reststepfast != 0)
        {
          if (!flgVirtual)
          {
            pos_fast += reststepfast;
            set_DACXY(portfast, pos_fast);
          } else
          { pos_fast += reststepfast; }

          sleep_us(conf_.delayF);
        }
//******************************************************************************
         sleep_us(conf_.pause);    // 50 CONST 50ms wait for start get data
//******************************************************************************     

        if (!flgVirtual)
        {
          getValuesFromAdc();
          vector_z.emplace_back((int16_t) spiBuf[ZPin]);  // считать Z из АЦП
        }
        else
        {
          vector_z.emplace_back(
              int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1)))); 
        }
      } //j

      for (uint32_t j = 0; j < stepfastline * nfastline; ++j) // возврат в начальную точку линии
      {
        if (!flgVirtual)
        {
          pos_fast -= conf_.diskretinstep;
          set_DACXY(portfast, pos_fast);
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
          set_DACXY(portfast, pos_fast);
        }
        else
        { pos_fast -= reststepfast; }

        sleep_us(conf_.delayF);
      }
      if ((nslowline - 1 - i) > 0)  //если непослелняя лниия
      {
        for (uint32_t n = 0; n < stepslowline; ++n) 
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            set_DACXY(portslow, pos_slow);
          } else
          { pos_slow += conf_.diskretinstep; }

          sleep_us(conf_.delayF);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow -= reststepslow;
            set_DACXY(portslow, pos_slow);
          } else
          { pos_slow -= reststepslow; }

          sleep_us(conf_.delayF);
        }
      }
    } //i

    afc.clear();
    afc = "code56";
    for (size_t j = 0; j < vector_z.size(); j++)     
    {
      afc += ',' + std::to_string(vector_z[j]);
    }
    vector_z.clear();
    afc += "\n";
    std::cout << afc;// посылка данных на ПК
    sleep_ms(100);
    afc.clear();

    stop_scan();  //возврат в начальную точку скана

    if (conf_.flgOneFrame == 1) { STOP = true; };
  } 
  blue();
  STOP=false;
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

void Scanner::scan_update(const Config &config)
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

void Scanner::move_toX0Y0(int x, int y,int delay) 
 //переместиться в начальную точку  скана из начальной точке предыдущего скана
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
      set_DACXY(1, ++pos_.x);
      sleep_us(delay);
    }
    while (pos_.x > point.x)
    {
      set_DACXY(1, --pos_.x);
      sleep_us(delay);
    }
    while (pos_.y < point.y)
    {
      set_DACXY(2, ++pos_.y);
      sleep_us(delay);
    }
    while (pos_.y > point.y)
    {
      set_DACXY(2, --pos_.y);
      sleep_us(delay);
    }
  } else
  {
    pos_.x = point.x;
    pos_.y = point.y;
    sleep_us(delay * (std::abs(pos_.x - point.x) + std::abs(pos_.y - point.y)));
  }
}

void Scanner::LID_move_toZ0(int lid_name, int f, int p, int n, int dir)  //отвестись в безопасную начальную точку по Z
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
void Scanner::positioningXYZ(int32_t *vector)
{
  uint8_t lid_name;
  uint16_t GATE_Z_MAX, GATE_Z_MIN;
  int8_t status;
  const int none = 30;
  const int ok = 3;
  const int touch = 2;
  int16_t ln;  
  bool ldir;
  int16_t p,f;
 // SET VALUE FROM RX_CORE
        lid_name=(uint8_t)vector[1]; //  int lid_name
               f=vector[2]; //  int f
               p=vector[3]; //  int p
              ln=abs(vector[4]); //  int n
            ldir=(bool)vector[5]; //  int dir
      GATE_Z_MAX=vector[6]; //  int Z gate max
      GATE_Z_MIN=vector[7]; //  int Z gate min
   //   pos_data[7] / //  0= SFM, 1=STM ;SICMAC-2; SICMDC-3;  device type
  //    pos_data[8]/ //  Voltage
      afc.clear();
        afc = "debug parameters pos update";
        for (int j = 0; j <= 6; ++j)
        {
          afc += ',' + std::to_string(vector[j]);
        }
        afc +="\n";
        std::cout << afc;
        afc.clear();
        sleep_ms(100);
  if (lid_name == 90 || lid_name == 95) //X,Y
  {
    while (!STOP) //LID_MOVE_UNTIL_STOP)
    {
      if (CONFIG_UPDATE)
      {
        CONFIG_UPDATE = false;
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
      afc.clear(); 
      afc = "code" + std::to_string(lid_name) + ',' + std::to_string(status) + ',' + std::to_string(ZValue) +
            ',' + std::to_string(SignalValue)+"\n";
      std::cout << afc;
      sleep_ms(100); 
      afc.clear();
    }
  }
  if (lid_name == 99) //Z
  {
    status = none;
    while (!STOP) //(LID_MOVE_UNTIL_STOP)
    {
    //   Z_STATE = true;  // 231215 ????
      if (CONFIG_UPDATE)
      { 
        CONFIG_UPDATE = false;
        ln = vector[1];
        GATE_Z_MAX = vector[2];
        GATE_Z_MIN = vector[3];
        ldir = 0;
        if (ln > 0) ldir = 1;
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
      if (!flgVirtual) 
      {
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
  STOP=false;
  afc.clear();
  afc = "code" + std::to_string(lid_name) + ',' + std::to_string(status) + ',' + std::to_string(ZValue) + ',' +
        std::to_string(SignalValue) + "\n";
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
  std::cout << "end\n";
  dark();
  //  sleep_ms(100); 
}
 static int ZMove( int Z0, int step, int mstep, int delay )   // st1 = +-1
	{
	  int16_t Zt;
    int16_t max  =  32767;
    int16_t min  = -32768;

	  Zt = Z0;
	  for (int16_t j=0; j< step; j++)
	  {
            Zt=Zt;
            if (mstep>0)
            {
             if (Zt==max-mstep) { Zt=(max-mstep);}
             else  Zt=(Zt+mstep);
            }
            else
            {
             if (Zt==min-mstep)  { Zt=(min-mstep);}
             else  Zt=(Zt+mstep);
            }
            for(int16_t k=0; k < delay; k++) { }// задержка в каждом дискрете

        //    Simple.cellWrite(M_scan_Z_offset, Zt); ///?????????????????????????
	  }
	  return(Zt);
	}
void Scanner::spectroscopyAZ(int32_t *vector) // спектроскопия Ampl-Z
{
/*
   params[0]:=(SpectrParams.Npoints);
   params[1]:=(round(SpectrParams.StartP*TransformUnit.Znm_d) );   //SFM
   params[2]:=(round(SpectrParams.Step*TransformUnit.Znm_d));
//   Threshold
     case  SpectrParams.flgType of
   I_Z:  params[3]:=(round((SpectrParams.LevelIZ)*0.01*round(abs(ApproachParams.SetPoint*TransformUnit.nA_d))));//stop approach i-z
   A_Z:  params[3]:=(round((100-SpectrParams.LevelSFM)*0.01*ApproachParams.UAMMax));//stop approach SFM  ampl-z
         end;
   params[4]:=(SpectrParams.T);
   params[5]:=(integer(flgmode));
 //  params[6]:=(apiType(round(SpectrParams.BiasV*TransformUnit.BiasV_d))); // add 22/10/25
*/
 const int16_t SFM=0;
 const int16_t STM=1;
      
 int16_t   NPoints=(int16_t )vector[1]; // n точек
 int16_t    ZStart=(int16_t )vector[2]; // ZStart
 int16_t     ZStep=(int16_t )vector[3]; // ZStep
 int16_t Threshold=(int16_t )vector[4]; // Threshold
 int16_t     Delay=(int16_t )vector[5]; // delay
 int16_t   flgModa=(int16_t )vector[6]; // flgmode stm,sfm;

 int16_t ampl;
 int16_t dlt,dacZ,dacZ0;

 int16_t MicrostepDelay=3;
  
  auto ptr = getValuesFromAdc();
     
  dacZ0=(int16_t)ptr[ZPin];
  dacZ=dacZ0;
 //start
  ampl=0;
 int16_t k=0;
 vectorA_Z.clear();

 freezeLOOP();
 sleep_ms(200);      
 dacZ = ZMove( dacZ, (-ZStart), 1, MicrostepDelay );
        
 for(int16_t i=0; i<NPoints; i++)     //сближение
  {
      sleep_ms(Delay);  //ms  add 30/05/22
      auto ptr = getValuesFromAdc();   
        switch (flgModa)
   {
    case SFM:  { ampl=(int16_t)ptr[AmplPin]; break;}  
    case STM:  { ampl=(int16_t)ptr[IPin];    break;}  
   }
     vectorA_Z.emplace_back(ampl);
     vectorA_Z.emplace_back(-(dacZ-dacZ0));
     vectorA_Z.emplace_back(1);

   if (flgModa==STM) //i  sfm    error corrected 14/09/21
   {
    if (ampl<0) ampl=-ampl;
    int imax=Threshold;
    if (imax<0) imax=-imax;
    if ((ampl<imax) &(i!=NPoints-1))
     {
         dacZ = ZMove( dacZ, (ZStep >> 16), -1, MicrostepDelay);
     }
     else break;
   };
   if (flgModa==SFM) //sfm  error corrected 14/09/21
   {
    if ((ampl>Threshold) &(i!=NPoints-1))
    {
       dacZ = ZMove( dacZ, (ZStep >> 16), -1, MicrostepDelay);
    }
    else break;
   }
 }  // for    i
    NPoints= k / 3;
    sleep_ms(300);

 // off = NPoints;
  for(int16_t i=NPoints; i>=1; i--)
  {
    sleep_ms(Delay); //add 30/05/22

     auto ptr = getValuesFromAdc();
     
        switch (flgModa)
   {
    case SFM:  { ampl=(int16_t)ptr[AmplPin]; break;}  
    case STM:  { ampl=(int16_t)ptr[IPin]; break;}  
   }
     vectorA_Z.emplace_back(ampl);
     vectorA_Z.emplace_back(-(dacZ-dacZ0));
     vectorA_Z.emplace_back(-1);
     dacZ = ZMove( dacZ, (ZStep >> 16), +1, MicrostepDelay);
  }

 //move to start point

  sleep_ms(300);
  if (dacZ-dacZ0 > 0) {dlt = dacZ-dacZ0;}
  else {dlt = -dacZ+dacZ0;}

  dacZ = ZMove( dacZ, dlt, -1, MicrostepDelay );
 
 //send data
   afc.clear();
   afc="code66";
   for (size_t m = 0; m < vectorA_Z.size(); m++)     // send data scanline
    {
      afc += ',' + std::to_string(vectorA_Z[m]);
    }
    afc += "\n";
    std::cout << afc;
    sleep_ms(200); //don't delete 
    afc.clear(); 
    vectorA_Z.clear();
 // разморозка состояния pid
    unfreezeLOOP();
    sleep_ms(500);
 //

}

void Scanner::spectroscopyIV(int32_t *vector)
{
    int i,j;
		int32_t  UBackup;
    int16_t  delay;
    int16_t  UPoints;
    int16_t  UCurves;
    int32_t  UStep;
    int32_t  UStart;
    int8_t   flgDev;
    int32_t  dacU;
    int32_t  start_step;
    int32_t  step;

    UPoints         = (int16_t) vector[1]; // n точек
    UCurves         = (int16_t) vector[2]; // m кривых
		UStart	      	=           vector[3]; // V начальное значение    
		UStep		        =           vector[4]; // V шаг 
  	delay           = (int16_t) vector[5]; // задержка в точке измерения
    flgDev          = (int8_t)  vector[6]; // прибор
    UBackup         =           vector[7]; // V текущее значение напряжения 
//start
  afc.clear();
  afc = "debug I_V parameters";
  for (int j = 0; j <= 6; ++j)
  {
    afc += ',' + std::to_string(vector[j]);
  }
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
 // add  turn off   FB     false!!!!!!!!!
  sleep_ms(300);
 // установка начального значения напряжения
      int16_t kk;
      int32_t dlt;
      start_step=100;
      dacU=UBackup;
      step=-start_step;
     int16_t nstep;
     int16_t rest;
//  снятие ВАХ
 for (j=0; j<UCurves; j++)
 {
      vectorI_V.clear();
      step=-start_step;
      dlt=(dacU-UStart);
      if (dlt<0)
      {
        step=start_step;
	      dlt=-dlt;
      }
        nstep=dlt / start_step;
        rest=dlt%start_step;
      for (kk=0; kk<nstep; kk++)
      {
       if (!flgVirtual) set_Bias(1,dacU);    
       sleep_ms(10);   
       dacU+=step;
      } 
       dacU+=rest;
      if (!flgVirtual)set_Bias(1,dacU);         
      sleep_ms(10);  
  
      for(i=0; i<UPoints; i++)
      {
       if (!flgVirtual) {set_Bias(1,dacU); }
       sleep_ms(delay);
       if (!flgVirtual)
       {
        auto ptr = getValuesFromAdc();
        vectorI_V.emplace_back(UStart+i*UStep);
        vectorI_V.emplace_back((int16_t)ptr[IPin]);
       }
       else
       {
        vectorI_V.emplace_back(UStart+i*UStep);
        vectorI_V.emplace_back(dacU);
       }
       dacU+=UStep;
      }

      sleep_ms(300);
    afc.clear();
    afc="code65";
   for (size_t m = 0; m < vectorI_V.size(); m++)     // send data scanline
    {
      afc += ',' + std::to_string(vectorI_V[m]);
    }
    afc += "\n";
    std::cout << afc;
    sleep_ms(200); //don't delete 
    afc.clear(); 
    vectorI_V.clear();
  //move to start point
  }// j Curves
    
    //возврат к исходному напряжению
     dacU-=UStep;
     step=-start_step;
     dlt=(dacU-UBackup);
     if (dlt<0)
     {
        step=start_step;
	      dlt=-dlt;
     }
      
  rest=dlt%start_step ;
  nstep=dlt/start_step;
 for (kk=0; kk<nstep; kk++)
 {
   set_Bias(1,dacU);  
   sleep_ms(10);
   dacU+=step;
 }
    dacU+=rest;
    set_Bias(1,dacU);  
    sleep_ms(10);
    set_Bias(1,UBackup);  
    sleep_ms(10);
//  add  turn on  FB   !!!!!!!!!!!!!!!
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )
  {
    sleep_ms(100);
    count++;
  } //ожидание ответа ПК для синхронизации
  TheadDone = false;
  std::cout << "end\n";  
}

void Scanner::approacphm(int32_t *vector) //uint16_t
{
  const int none = 30;
  const int ok = 3;
  const int touch = 2;
  const int stopdone = 1;
  uint16_t ZMaxValue = 32767;
  uint16_t SET_POINT, GATE_Z_MAX, GATE_Z_MIN;
  int16_t  freq, scv;//
  int16_t  GAIN, NSTEPS;
  uint16_t INTDELAY, SCANNERDECAY;
  uint8_t  flgDev;
  int32_t  Bias;
 
  // SET VALUE FROM RX_CORE
  SET_POINT      = vector[1]; // set point
  GATE_Z_MAX     = vector[2]; // max
  GATE_Z_MIN     = vector[3]; // min
  NSTEPS         = vector[4]; // steps 
  INTDELAY       = vector[5]; // initdelay
  GAIN           = vector[6]; // gain
  SCANNERDECAY   = vector[7]; // scannerDelay 
  freq           = vector[8]; // freq
  scv            = vector[9]; // scv
  flgDev         = (uint8_t)vector[10];//  0= SFM, 1=STM ;SICMAC-2; SICMDC-3;  device type
  Bias           = vector[11];// Voltage need for STM,SICM
 //need to add channel Bias ????
 //need to add channel SetPoint ????

  afc.clear();
  afc = "debug approach parameters 1 ";
  for (size_t j = 0; j < 9; j++)     // send info
  {
    afc += ',' + std::to_string(vector[j]);
  }
  afc += std::to_string(AmplPin) + ',' + std::to_string(ZPin) + "\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(200);
  set_SetPoint(0,SET_POINT); 
  if (flgDev!=0) set_Bias(1,Bias);  
  set_gainPID(GAIN);
  /*
  std::vector<int16_t> buf_params;
  buf_params.reserve(7);

  for (int i = 0; i < 7; ++i)  buf_params.push_back(vector[i]);
 */
  if (!flgVirtual)
  {
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
  afc ="code75," + std::to_string(buf_status[0]) + ',' + std::to_string(buf_status[1]) + ',' 
      +std::to_string(buf_status[2]) + "\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);

  while (true)
  {
    buf_status[0] = none;
    if (STOP)
    {
      STOP=false;
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
    if (CONFIG_UPDATE)
    {
      // log("config updated\n",flgDebugLevel);
      CONFIG_UPDATE = false;
      SET_POINT    = vector[1];
      GATE_Z_MAX   = vector[2];
      GATE_Z_MIN   = vector[3];
      NSTEPS       = vector[4];
      INTDELAY     = vector[5];
      GAIN         = vector[6];
      SCANNERDECAY = vector[7];
     
      if (flgDev!=0) set_Bias(1,Bias);  
      set_SetPoint(0,SET_POINT); //add 231214 
      set_gainPID(GAIN);
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
      auto ptr = getValuesFromAdc();
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
   // if (flgDebugLevel <= DEBUG_LEVEL);//log("Z = " + std::to_string(Z) + '\n',flgDebugLevel);
      buf_status[1] = ZValue;
      buf_status[2] = SignalValue;    
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
      retract();  //втянуть сканнер
      sleep_ms(SCANNERDECAY);
      linearDriver.activate(99, freq, scv, std::abs(NSTEPS), NSTEPS > 0);
      protract(); //вытянуть
    }
  }
  afc.clear();
  afc = "code75," + std::to_string(buf_status[0]) + ',' + std::to_string(buf_status[1]) + ',' +
        std::to_string(buf_status[2]) + "\n";
  std::cout << afc;
  afc.clear();
  //APPROACH = false;
  if (!flgVirtual)
  {
    protract();//вытянуть
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
//  afc = "code25";
  std::vector<int32_t> data;

  while ((scan_index++ < inBuf[0]))
  {
    if (!flgVirtual)
    {
      set_freq(inBuf[1]);
      sleep_ms(inBuf[4]);
   //   afc +=',' + std::to_string(inBuf[1]) + ',' +
  //        std::to_string((int16_t) getValuesFromAdc()[current_channel]);//+ ',';
      data.emplace_back((int32_t)inBuf[1]);
      data.emplace_back((int32_t)getValuesFromAdc()[current_channel]);
    }
    else
    {
      current_freq = inBuf[1];
      sleep_ms(inBuf[4]);
      signalvalue = (int16_t) std::round(a * (pow(M_E, -pow((current_freq - res_freq), 2) / 1000000))); //231126
    //  afc += ',' + std::to_string(current_freq) + ',' + std::to_string(signalvalue);//+',';
      data.emplace_back((int32_t)current_freq);
      data.emplace_back((int32_t)signalvalue);

    }
    sleep_ms(10);
    inBuf[1] += inBuf[2];
  }
 // std::cout << afc << '\n';
 // sleep_ms(100);
 // afc.clear();
  sendStrdata("code25",data);
  data.clear();
  current_channel = -1;
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )
  {
    sleep_ms(100);
    count++;
  } //ожидание ответа ПК для синхронизации
  TheadDone = false;
  std::cout << "end\n";
 // RESONANCE = false;
}

