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
void Scanner::sendStrData(std::string const& header)
{
 std::string afcc;
  afcc.clear();
  afcc=header; 
  afcc +="\n";
  std::cout << afcc;
  afcc.clear();
 // sleep_ms(100);
}

void Scanner::sendStrData(std::string const& header, std::vector<int32_t> &data, const uint16_t delay,const bool flg)
{
  std::string afcc;
  afcc.clear();
  afcc=header; 
   //for (auto & element :data) 
  for (size_t j = 0; j < data.size(); ++j)
  {
 // afc +=',' + std::to_string(element);
   afcc +=',' + std::to_string(data[j]);
  }
  afcc +="\n";
  std::cout << afcc;
  afcc.clear();
  if (flg) data.clear();
  sleep_ms(delay);
}

void Scanner::sendStrData(std::string const& header,std::vector<int16_t> &data, const uint16_t delay, const bool flg)
{
  std::string afcc;
  afcc.clear();
  afcc=header;
   //for (auto & element :data) 
  for (size_t j = 0; j < data.size(); ++j)
  {
 // afc +=',' + std::to_string(element);
   afcc +=',' + std::to_string(data[j]);
  }
  afcc +="\n";
  std::cout << afcc;
  afcc.clear();
  sleep_ms(delay);
  if (flg) data.clear();
}

void Scanner::sendStrData(std::string const& header,std::vector<uint16_t> &data, const uint16_t delay, const bool flg)
{
  std::string afcc;
  afcc.clear();
  afcc=header;
   //for (auto & element :data) 
  for (size_t j = 0; j < data.size(); ++j)
  {
 // afc +=',' + std::to_string(element);
   afcc +=',' + std::to_string(data[j]);
  }
  afcc +="\n";
  std::cout << afcc;
  afcc.clear();
  sleep_ms(delay);
  if (flg) data.clear();
}
void Scanner::readDATALin()
{
  data_LinX.clear();
  data_LinY.clear();
  size_t szx=vector[1]; //nx
  size_t szy=vector[2]; //ny
  debugdata.emplace_back(szx);
  debugdata.emplace_back(szy);
  sendStrData("debug linxny ",debugdata,400,true);
  for (size_t j = 0; j <szx; ++j)
  {
   data_LinX.emplace_back((uint16_t)vector[j+3]);
  }
//  sendStrData("debug linx ",data_LinX,400,false);
  for (size_t j = 0; j <szy; ++j)
  {
   data_LinY.emplace_back((uint16_t)vector[j+3+szx]);
  } 
//  sendStrData("debug liny ",data_LinY,400,false);
}
  
void Scanner::readADC()
{
  if (!flgVirtual)
  {
   auto ptr = getValuesFromAdc();
  //logger(ptr, 8);
   ZValue = (int16_t) ptr[ZPin];
      switch (vector[1]) //прибор
   {
        case SFM: //SFM
                {
                 SignalValue = (int16_t) ptr[AmplPin];
                 break;  
                } 
        case STM://STM
     case SICMDC://SICMDC  
                {
                 SignalValue = (int16_t) ptr[IPin];
                 break;  
                } 
   }         
        debugdata.emplace_back(ZValue);
        debugdata.emplace_back(SignalValue);
        debugdata.emplace_back(vector[1]);
        sendStrData("code12",debugdata,100,true);
  } 
  else
  {
        debugdata.emplace_back(ZValue);
        debugdata.emplace_back(SignalValue);
        debugdata.emplace_back(vector[1]);
        sendStrData("code12",debugdata,100,true);
  }
}
void Scanner::scanner_retract_protract(int port, int flg) // port  6  1- втянуть,  0-вытянуть
 {
  flg == 1 ? io_ports[port].enable() : io_ports[port].disable();
  afc.clear();
  if (flg == 1) { afc = "debug scanner retract " + std::to_string(port);}
  else          { afc = "debug scanner protract" + std::to_string(port);}
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100); 
 } 

void Scanner::retract() //втянуть
{
  io3_1.enable();  //  port 6   элемент массива портов 
}
void Scanner::retract(uint16_t HeightJump) //втянуть на H
{
 freezeLOOP(100);
 set_DACZ(0,-HeightJump); 
}

void Scanner::protract() //вытянуть
{
  io3_1.disable();  //port 6
}
void Scanner::LOOP_freeze_unfreeze(int port, int flg) // port virtual 5
{
//flg == 1 ? io_ports[port - 1].enable() : io_ports[port - 1].disable();
 flg == 1 ? io_ports[port].enable() : io_ports[port].disable();
}
void Scanner::freezeLOOP(uint16_t delay)    // заморозить ПИД
{
  io3_0.enable(); // 5 элемент массива портов ???
  sleep_ms(delay);
}

void Scanner::unfreezeLOOP(uint16_t delay)  // разморозить ПИД
{
  io3_0.disable();  // 5 элемент массива портов ???
  sleep_ms(delay);
}

bool Scanner::getHoppingFlg() //получить флаг установлен ли флаг сканирования прыжками
{
  return (bool)conf_.flgHoping;
}
bool Scanner::getLinearFlg()
{
  return (bool)conf_.flgLin;
}
void Scanner::start_scan(std::vector<int32_t> &vector) //сканирование
{
  const int8_t oneline=11;
/*
struct Config
{
  uint16_t nPoints_x;        // точек по оси  X                              1
  uint16_t nPoints_y;        // точек по оси  Y                              2 
  uint8_t  path;             // сканирование  0 - по оси X, 1 - по оси Y     3
  uint8_t  method;           // что измерять Topo=0,Phase=1, Ampl=2...       4
  uint16_t delayF;           // задержка при сканировании вперёд             5
  uint16_t delayB;           // задержка при сканировании назад              6
  uint16_t betweenPoints_x;  // расстояние между точками по X в дискретах    7 
  uint16_t betweenPoints_y;  // расстояние между точками по Y в дискретах    8 
  uint8_t  size;             // size=1  -Z; size=2 - Z,Амплитуда             9
  uint8_t  Ti;               // усиление ПИД                                10
  uint16_t diskretinstep;    // размер шага в дискретах                     11
  uint16_t pause;            // время ожидания в точке измерения  мксек     12  
  uint8_t  flgLin;           // флаг линеализации                           13   
  int16_t  lineshift;        //сдвиг линии -учет неортогональности сканнера 14
  uint8_t  flgOneFrame;      // быстрое сканирование один кадр=1            15
  uint8_t  flgHoping;        // сканирование прыжками                       16
  uint16_t HopeDelay;        // задержка в точке измерения при прыжках      17
};
*/
  prev_point = pos_; //запоминание начальной точки скана
  vector_data.clear();
  for (int j = 1; j <= 17; ++j)
  {
    debugdata.emplace_back(vector[j]);
  }
  debugdata.emplace_back(pos_.x);
  debugdata.emplace_back(pos_.y);

  sendStrData("debug scan parameters",debugdata,100,true);

  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepsslowline, stepsfastline;
  uint8_t  portx =0;// 1;
  uint8_t  porty =1;// 2;
  uint8_t  portfast;
  uint8_t  portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;
/*  
  stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
  stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
  reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
  reststepy = conf_.betweenPoints_y % conf_.diskretinstep;
  debugdata.emplace_back(stepsx);
  debugdata.emplace_back(stepsy);
  debugdata.emplace_back(reststepx);
  debugdata.emplace_back(reststepy);
  sendStrData("debug scan parameters stepsxy  ",debugdata,100);
*/
   DrawDone=true;
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
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast = reststepx;
        reststepslow = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast = reststepy;
        reststepslow = reststepx;
        break;
      }
    }
    for (uint32_t j = 0; j < nfastline; ++j)
    {
      for (uint32_t k = 0; k < stepsfastline; ++k)  
      {
        if (!flgVirtual)
        {
          set_DACXY(portfast, pos_fast);
          pos_fast += conf_.diskretinstep;
        } 
        else
        { pos_fast += conf_.diskretinstep; }
        sleep_us(conf_.delayF);
      }
      if (reststepfast != 0)// добирание остатка
      {
        if (!flgVirtual)
        {
          pos_fast += reststepfast;
          set_DACXY(portfast, pos_fast);
        } 
        else
        { pos_fast += reststepfast; }
        sleep_us(conf_.delayF);
      }

      //******************************************************************************
      sleep_us(conf_.pause);    // 50 CONST 50ms wait for start get data
      //*******************************************************************************
      if (!flgVirtual)
      {
        getValuesFromAdc();
        vector_data.emplace_back(ZMaxValue-(int16_t) spiBuf[ZPin]);  // get Z from adc
        if (conf_.size == 2)
          switch (conf_.method)
          {
            case 3://phase 
            {
              vector_data.emplace_back((int16_t) spiBuf[1]); 
              break;
            }
            case 4://ampl
            {
              vector_data.emplace_back((int16_t) spiBuf[AmplPin]); 
              break;
            }
            case 7://current
            {
              vector_data.emplace_back((int16_t) spiBuf[IPin]); 
              break;
            }
          }
      }
      else
      {
   //    if (conf_.method!=oneline) vector_data.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));  // get Z from adc
   //    else   vector_data.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * 0 * 0.1)))); 
       double_t w;
       w= 10*M_PI/(nfastline);   
       if (conf_.method!=oneline) vector_data.emplace_back(int16_t(10000.0 * (sin(w*j) + sin(w* i ))));  // get Z from adc
   //    if (conf_.method!=oneline) vector_data.emplace_back(int16_t(10000.0 * (sin(w*pos_fast) + sin(w*pos_slow ))));  // get Z from adc
       else   vector_data.emplace_back(int16_t(10000.0 * (sin(w * j)))); 
        if (conf_.size == 2)  //дополнительный сигнал
        {
           vector_data.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));
        }
      }
    }
// move  back  add 24/01/22 ////////////////////////////////////
    stepsx    = (uint16_t) conf_.betweenPoints_x*conf_.nPoints_x / conf_.diskretinstep;
    stepsy    = (uint16_t) conf_.betweenPoints_y*conf_.nPoints_y / conf_.diskretinstep;
    reststepx = (uint16_t) conf_.betweenPoints_x*conf_.nPoints_x % conf_.diskretinstep;
    reststepy = (uint16_t) conf_.betweenPoints_y*conf_.nPoints_y % conf_.diskretinstep;

    switch (conf_.path)
    {
      case 0://X+
      {
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast = reststepx;
        reststepslow = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast = reststepy;
        reststepslow = reststepx;
        break;
      }
    }
 /////////////////////////////////////////////   
   // for (uint32_t l = 0; l < stepfastline * nfastline; ++l) //com 240122   move  back
    for (uint32_t l = 0; l < stepsfastline; ++l) // move  back
    {
      if (!flgVirtual)
      {
        pos_fast -= conf_.diskretinstep;
        set_DACXY(portfast, pos_fast);
      }
      else  { pos_fast -= conf_.diskretinstep; }
      sleep_us(conf_.delayB);
    }

    if (reststepfast != 0)// добирание остатка
    {
      if (!flgVirtual)
      {
        pos_fast -= reststepfast;
        set_DACXY(portfast, pos_fast);
      }
      else  { pos_fast -= reststepfast; }

      sleep_us(conf_.delayF);
    }
    int16_t count0 = 0;
    while ((!DrawDone) || (count0<20) )//ожидание ответа ПК для синхронизации
    {
     sleep_ms(10);
     count0++;
    } 
    DrawDone = false;
    sendStrData("code50",vector_data,60,true); //100
 
    if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE = false;
      conf_.delayF  = vector[1];
      conf_.delayB  = vector[2];
      set_GainPID(vector[3]);
      sleep_ms(100);              
      conf_.diskretinstep = vector[4]; 
 
      for (int j = 1; j <= 3; ++j)
      {
        debugdata.emplace_back(vector[j]);
      }
      sendStrData( "debug parameters update",debugdata,100,true);
     stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
     stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
     reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
     reststepy = conf_.betweenPoints_y % conf_.diskretinstep;
     switch (conf_.path)
     {
      case 0://X+
      {
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast = reststepx;
        reststepslow = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast = reststepy;
        reststepslow = reststepx;
        break;
      }
      //    dark();
     }
    }
    if (STOP)   // stop
    {
      STOP = false;
      sleep_ms(100);
      sendStrData("stopped");
      break;
    }
    if ((nslowline - 1 - i) > 0)  //если непоследняя линия
    {
      if (conf_.method != oneline) //не сканирование по одной линии
      {
        for (uint32_t n = 0; n < stepsslowline; ++n) // переход на следующую линию
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            set_DACXY(portslow, pos_slow);
          }
          else { pos_slow += conf_.diskretinstep; }
          sleep_us(conf_.delayF);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow -= reststepslow;
            set_DACXY(portslow, pos_slow);
          } 
          else { pos_slow -= reststepslow; }
          sleep_us(conf_.delayF);
        }
      }
    }
  } 
  blue();
  switch (conf_.path) ///???????
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
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  green();
  sendStrData("end"); 
  activateDark();
}
void Scanner::start_scanlin(std::vector<int32_t> &vector) //сканирование
{
  const int8_t oneline=11;

  prev_point = pos_; //запоминание начальной точки скана
  vector_data.clear();
  for (int j = 1; j <= 17; ++j)
  {
    debugdata.emplace_back(vector[j]);
  }
  debugdata.emplace_back(pos_.x);
  debugdata.emplace_back(pos_.y);

  sendStrData("debug scan lin parameters",debugdata,200,true);//200

  //sendStrData("debug linx ",data_LinX,100,false);

  //sendStrData("debug liny ",data_LinY,100,false);

  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepsslowline, stepsfastline;
  uint8_t  portx = 0;//1;
  uint8_t  porty = 1;//2;
  uint8_t  portfast;
  uint8_t  portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;

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
      break;
    }
  }
  for (size_t i = 0; i < nslowline; ++i)
  {
    for (uint32_t j = 0; j < nfastline; ++j)
    {
    switch (conf_.path)
    {
      case 0://X+
      {
        stepsx = (uint16_t) data_LinX[j] / conf_.diskretinstep;
        reststepx = (uint16_t) data_LinX[j] % conf_.diskretinstep;
        stepsfastline = stepsx;
        reststepfast = reststepx;
        break;
      }
      case 1: //Y+
      {
        stepsy = (uint16_t) data_LinY[j] / conf_.diskretinstep;
        reststepy = (uint16_t) data_LinY[j] % conf_.diskretinstep;
        stepsfastline = stepsy;
        reststepfast = reststepy;
        break;
      }
    }
     for (uint32_t k = 0; k < stepsfastline; ++k)  
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
        vector_data.emplace_back(ZMaxValue-(int16_t) spiBuf[ZPin]);  // get Z from adc
        if (conf_.size == 2)
          switch (conf_.method)
          {
            case 3://phase 
            {
               vector_data.emplace_back((int16_t) spiBuf[1]); 
              break;
            }
            case 4://ampl
            {
               vector_data.emplace_back((int16_t) spiBuf[AmplPin]); 
              break;
            }
            case 7://current
            {
               vector_data.emplace_back((int16_t) spiBuf[IPin]); 
              break;
            }
          }
      }
      else
      {
   //    if (conf_.method!=oneline) vector_data.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));  // get Z from adc
   //    else   vector_data.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * 0 * 0.1)))); 
         double_t w;
         w= 10*M_PI/(nfastline);   
         if (conf_.method!=oneline) vector_data.emplace_back(int16_t(10000.0 * (sin(w*j) + sin(w* i ))));  // get Z from adc
     
    //  w= 10*M_PI/(nfastline*);   
    //   if (conf_.method!=oneline) vector_data.emplace_back(int16_t(10000.0 * (sin(w*j) + sin(w* i ))));  // get Z from adc
   //    else   vector_data.emplace_back(int16_t(10000.0 * (sin(w * j)))); 
        if (conf_.size == 2)  //дополнительный сигнал
        {
           vector_data.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));
        }
      }
    }
// move  back  add 24/01/22 ////////////////////////////////////
    stepsx    = (uint16_t) data_LinX[conf_.nPoints_x] / conf_.diskretinstep;
    stepsy    = (uint16_t) data_LinY[conf_.nPoints_y] / conf_.diskretinstep;
    reststepx = (uint16_t) data_LinX[conf_.nPoints_x] % conf_.diskretinstep;
    reststepy = (uint16_t) data_LinY[conf_.nPoints_y] % conf_.diskretinstep;

    switch (conf_.path)
    {
      case 0://X+
      {
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast = reststepx;
        reststepslow = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast = reststepy;
        reststepslow = reststepx;
        break;
      }
    }
 /////////////////////////////////////////////   
   // for (uint32_t l = 0; l < stepfastline * nfastline; ++l) //com 240122   move  back
    for (uint32_t l = 0; l < stepsfastline; ++l) // move  back
    {
      if (!flgVirtual)
      {
        pos_fast -= conf_.diskretinstep;
        set_DACXY(portfast, pos_fast);
      }
      else { pos_fast -= conf_.diskretinstep; }
      sleep_us(conf_.delayB);
    }
    if (reststepfast != 0)// добирание остатка
    {
      if (!flgVirtual)
      {
        pos_fast -= reststepfast;
        set_DACXY(portfast, pos_fast);
      }
      else { pos_fast -= reststepfast; }
      sleep_us(conf_.delayF);
    }
    int16_t count0 = 0;
    while ((!DrawDone) || (count0<20) )//ожидание ответа ПК для синхронизации
    {
     sleep_ms(10);
     count0++;
    } 
    DrawDone = false;
    sendStrData("code50",vector_data,40,true); //100
     if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE = false;
      conf_.delayF        = vector[1];
      conf_.delayB        = vector[2];
      set_GainPID(vector[3]);
      sleep_ms(100);              
      conf_.diskretinstep = vector[4]; 
       for (int j = 1; j <= 3; ++j)
      {
        debugdata.emplace_back(vector[j]);
      }
      sendStrData( "debug parameters update",debugdata,100,true);
      //    dark();
    }
    if (STOP)   // stop
    {
      STOP = false;
      sleep_ms(100);
      sendStrData("stopped");
      break;
    }
    // next line
    if ((nslowline - 1 - i) > 0)  //если не последняя линия
    {
      if (conf_.method != oneline) //не сканирование по одной линии
      {
        switch (conf_.path)
       {
         case 0://X+
        {
            stepsy = (uint16_t) data_LinY[i] / conf_.diskretinstep;
         reststepy = (uint16_t) data_LinY[i] % conf_.diskretinstep;
         stepsslowline = stepsy;
         reststepslow  = reststepy;
         break;
        }
        case 1: //Y+
        {
         stepsx    = (uint16_t) data_LinX[i] / conf_.diskretinstep;
         reststepx = (uint16_t) data_LinX[i] % conf_.diskretinstep;
         stepsslowline = stepsx;
         reststepslow = reststepx;
         break;
        }
       }
        for (uint32_t n = 0; n < stepsslowline; ++n) // переход на следующую линию
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            set_DACXY(portslow, pos_slow);
          } 
          else { pos_slow += conf_.diskretinstep; }
          sleep_us(conf_.delayF);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow -= reststepslow;
            set_DACXY(portslow, pos_slow);
          }
          else { pos_slow -= reststepslow; }
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
  sleep_ms(300); //200
  //red();
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  green();
  sendStrData("end"); 
  activateDark();
}
void Scanner::start_hopingscan(std::vector<int32_t> &vector)
{
  const int8_t oneline=11;
  prev_point = pos_; //запоминание начальной точки скана
  vector_data.clear();
  for (int j = 1; j <= 23; ++j)
  {
    debugdata.emplace_back(vector[j]);
  }
  sendStrData("debug hoping scan parameters",debugdata,100,true);
  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepsslowline, stepsfastline;
  uint8_t  portx =0;// 1;
  uint8_t  porty =1;// 2;
  uint8_t  portfast;
  uint8_t  portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;
  uint16_t ZJump;
  int16_t  ISatCur;
  int16_t  ISatCurPrev;

  bool  flgMaxJump;
  
  flgMaxJump=(conf_.HopeZ==0);

    //std::random_device rd;
    // Create a Mersenne Twister pseudo-random number generator
    //std::mt19937 gen(rd());
    
    // Create a uniform distribution from 1 to 100
    std::uniform_int_distribution<int> dis(1, 100);

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
      break;
    }
  }
      if (!flgVirtual)
      {
        getValuesFromAdc();
        ISatCurPrev=(int16_t) spiBuf[IPin];
      }
      else
      {
        ISatCurPrev=(int16_t)round(conf_.SetPoint);
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
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast  = reststepx;
        reststepslow  = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast  = reststepy;
        reststepslow  = reststepx;
        break;
      }
    }  
   for (uint32_t j = 0; j < nfastline; ++j)
    {
      if (!flgVirtual)
      {
        if (flgMaxJump)  retract();
        else retract(ZJump) ;
      }   
      sleep_us(50);
      for (uint32_t k = 0; k < stepsfastline; ++k) 
      {
        if (!flgVirtual)
        {
          set_DACXY(portfast, pos_fast);
          pos_fast += conf_.diskretinstep;
        } 
        else { pos_fast += conf_.diskretinstep; }
        sleep_us(conf_.delayF);
      }
      if (reststepfast != 0)
      {
        if (!flgVirtual)
        {
          pos_fast += reststepfast;
          set_DACXY(portfast, pos_fast);
        }
        else { pos_fast += reststepfast; }
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
        vector_data.emplace_back(ZMaxValue-(int16_t) spiBuf[ZPin]);     // считать  Z 
        switch (conf_.method)
          //added signal  Const  BackPass=2;    //PM  Const  Phase=3;  Const  UAM=4;   //Force Image
        {
          case 3://phase !!!!
          {
            vector_data.emplace_back((int16_t) spiBuf[1]);
            break;
          }
          case 4://ampl
          {
            vector_data.emplace_back((int16_t) spiBuf[AmplPin]);
            break;
          }
          case 7://current
          {
            vector_data.emplace_back((int16_t) spiBuf[IPin]);
             break;
          }
        }
      }
      else
      {
    //    vector_data.emplace_back(
    //        int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));  // get Z from adc
        double_t w;
       w= 10*M_PI/(nfastline);   
       vector_data.emplace_back(int16_t(10000.0 * (sin(w*j) + sin(w* i ))));  // get Z from adc
     if (conf_.size == 2)                               // added signal
        {
          vector_data.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));
        }
      }
    } //fast line
     
      //move to the start line point   
       if (!flgVirtual)
      {
        if (flgMaxJump)  retract();
      } 
      sleep_us(50);

      if (!flgVirtual)
      {
       pos_fast -= conf_.diskretinstep * stepsfastline * nfastline;
       set_DACXY(portfast, pos_fast);
      }
      else
      { pos_fast -= conf_.diskretinstep * stepsfastline * nfastline; }
      sleep_us(conf_.delayB);
      if (reststepfast != 0)
      {
       if (!flgVirtual)
       {
        pos_fast -= reststepfast;
        set_DACXY(portfast, pos_fast);
       }
       else { pos_fast -= reststepfast; }
       sleep_us(conf_.delayF);
      }
      int16_t count0 = 0;
    while ((!DrawDone) || (count0<20))//ожидание ответа ПК для синхронизации
    {
     sleep_ms(10);
     count0++;
    } 
    DrawDone = false;
      if (!flgVirtual)
      {
        getValuesFromAdc();
        ISatCur=(int16_t) spiBuf[IPin];
        vector_data.emplace_back(ISatCur);
      }
      else
      {
       uint16_t random_num =i;   
       ISatCur=ISatCur-100*random_num;
       vector_data.emplace_back(ISatCur);
      }
// auto correction setpoint for sicm
     if (conf_.flgAutoUpdateSP) 
     {
       if (conf_.flgAutoUpdateSPDelta) 
       {
         if (abs((ISatCurPrev-ISatCur)/ISatCurPrev)>0.01*conf_.ThresholdAutoUpdate) 
         {
          conf_.SetPoint=round(ISatCur*conf_.KoeffCorrectISat*0.01 );
          ISatCurPrev=ISatCur;
          set_SetPoint(0,conf_.SetPoint);
          sleep_ms(conf_.HopeDelay);
         }
       }
       else
       { 
        conf_.SetPoint=round(ISatCur*conf_.KoeffCorrectISat*0.01 );
        set_SetPoint(0,conf_.SetPoint);
        ISatCurPrev=ISatCur;
        sleep_ms(conf_.HopeDelay);
       }
     }
     
     vector_data.emplace_back(round(conf_.SetPoint));
     sendStrData("code50",vector_data,60,true); //send data

    if (STOP)  // stop
    {
      STOP = false;
      sendStrData("stopped");
      break;
    }
    if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE              = false;
      conf_.delayF               = vector[1];
      conf_.delayB               = vector[2];
      set_GainPID(vector[3]);
      conf_.HopeDelay            = vector[4];
      conf_.HopeZ                = vector[5];
      conf_.flgAutoUpdateSP      = vector[6];; // автообновление опоры на каждой линии                     19
      conf_.flgAutoUpdateSPDelta = vector[7];; // обновление опоры , если изменение тока превысило порог 20
      conf_.ThresholdAutoUpdate  = vector[8];; // изменения опоры, если изменение тока превысило порог     21
      conf_.KoeffCorrectISat     = vector[9];  // опора  %  от тока насыщения        
      flgMaxJump=(conf_.HopeZ==0);
      sleep_ms(100);   
      for (int j = 1; j <= 9; ++j)
      {
        debugdata.emplace_back(vector[j]);
      }
      sendStrData("debug parameters update",debugdata,100,true);
  
      stepsx = (uint16_t) conf_.betweenPoints_x / conf_.diskretinstep;
      stepsy = (uint16_t) conf_.betweenPoints_y / conf_.diskretinstep;
      reststepx = conf_.betweenPoints_x % conf_.diskretinstep;
      reststepy = conf_.betweenPoints_y % conf_.diskretinstep;
       switch (conf_.path)
     {
      case 0://X+
      {
        stepsslowline = stepsy;
        stepsfastline = stepsx;
        reststepfast = reststepx;
        reststepslow = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsslowline = stepsx;
        stepsfastline = stepsy;
        reststepfast = reststepy;
        reststepslow = reststepx;
        break;
      }
     }
    }
 
    //next line
     if ((nslowline - 1 - i) > 0)  //если не последняя линия
     {
      if (conf_.method !=oneline) //не сканирование по одной линии
      {
        for (uint32_t n = 0; n < stepsslowline; ++n) // переход на следующую линию
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            set_DACXY(portslow, pos_slow);
          }
          else { pos_slow += conf_.diskretinstep; }
          sleep_us(conf_.delayF);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow -= reststepslow;
            set_DACXY(portslow, pos_slow);
          }
          else { pos_slow -= reststepslow; }
          sleep_us(conf_.delayF);
        }
      }
     }
  } 

  //end scanning 
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
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  conf_.flgHoping=0;
  sendStrData("end");
  activateDark();
}
void Scanner::start_hopingscanlin(std::vector<int32_t> &vector)
{
  const int8_t oneline=11;
  prev_point = pos_; //запоминание начальной точки скана
  vector_data.clear();
  for (int j = 1; j <= 23; ++j)
  {
    debugdata.emplace_back(vector[j]);
  }
  sendStrData("debug hoping scan parameters",debugdata,100,true);
  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepsslowline, stepsfastline;
  uint8_t  portx = 0;// 1;
  uint8_t  porty = 1;// 2;
  uint8_t  portfast;
  uint8_t  portslow;
  uint16_t pos_fast;
  uint16_t pos_slow;
  uint16_t ZJump;
  bool  flgMaxJump;
  int16_t  ISatCur;
  int16_t  ISatCurPrev;

  flgMaxJump=(conf_.HopeZ==0);
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
      stepsslowline = stepsy;
      stepsfastline = stepsx;
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
      stepsslowline = stepsx;
      stepsfastline = stepsy;
      reststepfast = reststepy;
      reststepslow = reststepx;
      break;
    }
  }

  if (!flgVirtual)
     {
        getValuesFromAdc();
        ISatCurPrev=(int16_t) spiBuf[IPin];
     }
     else
     {
        ISatCurPrev=(int16_t)round(conf_.SetPoint);
     }

  for (uint32_t i = 0; i < nslowline; ++i)
  { 
   for (uint32_t j = 0; j < nfastline; ++j)
    {
      switch (conf_.path)
     {
      case 0://X+
      {
           stepsx = (uint16_t) data_LinX[j] / conf_.diskretinstep;
        reststepx = (uint16_t) data_LinX[j] % conf_.diskretinstep;
        stepsfastline = stepsx;
         reststepfast = reststepx;
        break;
      }
      case 1: //Y+
      {
           stepsy = (uint16_t) data_LinY[j] / conf_.diskretinstep;
        reststepy = (uint16_t) data_LinY[j] % conf_.diskretinstep;
        stepsfastline = stepsy;
         reststepfast = reststepy;
        break;
      }
     }
      if (!flgVirtual)
      {
        if (flgMaxJump)  retract();
        else retract(ZJump) ;
      }   
      sleep_us(50);
      for (uint32_t k = 0; k < stepsfastline; ++k) 
      {
        if (!flgVirtual)
        {
          set_DACXY(portfast, pos_fast);
          pos_fast += conf_.diskretinstep;
        }
        else { pos_fast += conf_.diskretinstep; }
        sleep_us(conf_.delayF);
      }
      if (reststepfast != 0)
      {
        if (!flgVirtual)
        {
          pos_fast += reststepfast;
          set_DACXY(portfast, pos_fast);
        }
        else { pos_fast += reststepfast; }

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
        vector_data.emplace_back(ZMaxValue-(int16_t) spiBuf[ZPin]);     // считать  Z 
        switch (conf_.method)
          //added signal  Const  BackPass=2;    //PM  Const  Phase=3;  Const  UAM=4;   //Force Image
        {
          case 3://phase !!!!
          {
            vector_data.emplace_back((int16_t) spiBuf[1]);
            break;
          }
          case 4://ampl
          {
            vector_data.emplace_back((int16_t) spiBuf[AmplPin]);
            break;
          }
          case 7://current
          {
            vector_data.emplace_back((int16_t) spiBuf[IPin]);
            break;
          }
        }
      } else
      {
    //    vector_data.emplace_back(
    //        int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));  // get Z from adc
        double_t w;
       w= 10*M_PI/(nfastline);   
       vector_data.emplace_back(int16_t(10000.0 * (sin(w*j) + sin(w* i ))));  // get Z from adc
     if (conf_.size == 2)                               // added signal
        {
          vector_data.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1))));
        }
      }
    }
     if (!flgVirtual)
     {
        if (flgMaxJump)  retract();
        //else retract(ZJump) ;
     } 
    sleep_us(50);
// move back
    switch (conf_.path)
    {
      case 0://X+
      {
        stepsx    = (uint16_t) data_LinX[conf_.nPoints_x] / conf_.diskretinstep;
        reststepx = (uint16_t) data_LinX[conf_.nPoints_x] % conf_.diskretinstep;
        stepsfastline = stepsx;
        reststepfast = reststepx;
        break;
      }
      case 1: //Y+
      {
        stepsy    = (uint16_t) data_LinY[conf_.nPoints_y] / conf_.diskretinstep;
        reststepy = (uint16_t) data_LinY[conf_.nPoints_y] % conf_.diskretinstep;
        stepsfastline = stepsy;
        reststepfast = reststepy;
        break;
      }
    }
    if (!flgVirtual)
    {
      pos_fast -= conf_.diskretinstep * stepsfastline * nfastline;
      set_DACXY(portfast, pos_fast);
    }
    else
    {
      pos_fast -= conf_.diskretinstep * stepsfastline * nfastline; 
    }
    
     sleep_us(conf_.delayB);

     if (reststepfast != 0)
     {
      if (!flgVirtual)
      {
        pos_fast -= reststepfast;
        set_DACXY(portfast, pos_fast);
      } 
      else { pos_fast -= reststepfast; }

      sleep_us(conf_.delayF);
     }
     int16_t count0 = 0;
     while ((!DrawDone) || (count0<20) )//ожидание ответа ПК для синхронизации
     {
      sleep_ms(10);
      count0++;
     } 
     DrawDone = false;
     if (!flgVirtual)
     {
        getValuesFromAdc();
        ISatCur=(int16_t) spiBuf[IPin];
        vector_data.emplace_back(ISatCur);
     }
     else
     {
       uint16_t random_num =i;   
       ISatCur=ISatCur-100*random_num;
       vector_data.emplace_back(ISatCur);
     }
// auto correction setpoint for sicm
     if (conf_.flgAutoUpdateSP) 
     {
       if (conf_.flgAutoUpdateSPDelta) 
       {
         if (abs((ISatCurPrev-ISatCur)/ISatCurPrev)>0.01*conf_.ThresholdAutoUpdate) 
         {
          conf_.SetPoint=round(ISatCur*conf_.KoeffCorrectISat*0.01 );
          ISatCurPrev=ISatCur;
          set_SetPoint(0,conf_.SetPoint);
          sleep_ms(conf_.HopeDelay);
         }
       }
       else
       { 
        conf_.SetPoint=round(ISatCur*conf_.KoeffCorrectISat*0.01 );
        set_SetPoint(0,conf_.SetPoint);
        ISatCurPrev=ISatCur;
        sleep_ms(conf_.HopeDelay);
       }
     }    
     vector_data.emplace_back(round(conf_.SetPoint));
     
     sendStrData("code50",vector_data,60,true);

    if (CONFIG_UPDATE)
    {
      CONFIG_UPDATE              = false;
      conf_.delayF               = vector[1];
      conf_.delayB               = vector[2];
      set_GainPID(vector[3]);
      conf_.HopeDelay            = vector[4];
      conf_.HopeZ                = vector[5];
      conf_.flgAutoUpdateSP      = vector[6];; // автообновление опоры на каждой линии                     19
      conf_.flgAutoUpdateSPDelta = vector[7];; // обновление опоры , если изменение тока превысило порог 20
      conf_.ThresholdAutoUpdate  = vector[8];; // изменения опоры, если изменение тока превысило порог     21
      conf_.KoeffCorrectISat     = vector[9];  // опора  %  от тока насыщения        
      flgMaxJump=(conf_.HopeZ==0);
      sleep_ms(100);   
      for (int j = 1; j <= 9; ++j)
      {
        debugdata.emplace_back(vector[j]);
      }
      sendStrData("debug parameters update",debugdata,100,true);
    }
    if (STOP)  // stop
    {
      STOP = false;
 //     sleep_ms(100);  //24/02/02
      sendStrData("stopped");
      break;
    }
///move next line
    switch (conf_.path)
    {
      case 0://X+
      {
        stepsy    = (uint16_t) data_LinY[i] / conf_.diskretinstep;
        reststepy = (uint16_t) data_LinY[i] % conf_.diskretinstep;
        stepsslowline = stepsy;
        reststepslow = reststepy;
        break;
      }
      case 1: //Y+
      {
        stepsx    = (uint16_t) data_LinX[i] / conf_.diskretinstep;
        reststepx = (uint16_t) data_LinX[i] % conf_.diskretinstep;
        stepsslowline = stepsx;
        reststepslow = reststepx;
        break;
      }
    }
    if ((nslowline - 1 - i) > 0)  //если не последняя линия
    {
     if (conf_.method !=oneline) //не сканирование по одной линии
     { 
        for (uint32_t n = 0; n < stepsslowline; ++n) // переход на следующую линию
        {
          if (!flgVirtual)
          {
            pos_slow += conf_.diskretinstep;
            set_DACXY(portslow, pos_slow);
          } 
          else { pos_slow += conf_.diskretinstep; }
          sleep_us(conf_.delayF);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow -= reststepslow;
            set_DACXY(portslow, pos_slow);
          }
          else { pos_slow -= reststepslow; }
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
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  conf_.flgHoping=0;
  sendStrData("end");
  activateDark();
} //hoppinglin

void Scanner::start_fastscan(std::vector<int32_t> &vector)
{
  scan_update({
               static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]),
               static_cast<uint8_t>(vector[3]),  static_cast<uint8_t>(vector[4]),
               static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
               static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
               static_cast<uint8_t>(vector[9]),  static_cast<uint8_t>(vector[10]),
               static_cast<uint16_t>(vector[11]),static_cast<uint16_t>(vector[12]),
               static_cast<uint8_t>(vector[13]), static_cast<int16_t>(vector[14]),  //add 240122            
               static_cast<uint8_t>(vector[15]), static_cast<uint8_t>(vector[16]),
               static_cast<uint16_t>(vector[17])
              }
             );  

  prev_point = pos_; //запоминание начальной точки скана
  vector_data.clear();
  for (int j = 1; j <= 17; ++j)
  {
    debugdata.emplace_back(vector[j]);
  }
  debugdata.emplace_back(pos_.x);
  debugdata.emplace_back(pos_.y);
  debugdata.emplace_back(conf_.flgOneFrame);
  sendStrData("debug fastscan parameters",debugdata,100,true);

  uint16_t stepsx;
  uint16_t stepsy;
  uint16_t reststepfast;
  uint16_t reststepslow;
  uint16_t reststepx;
  uint16_t reststepy;
  uint16_t nfastline, nslowline;
  uint16_t stepslowline, stepfastline;
  uint8_t  portx = 0;//1;
  uint8_t  porty = 1;//2;
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
          }
          else { pos_fast += conf_.diskretinstep; }
          sleep_us(conf_.delayF);
        }
        if (reststepfast != 0)
        {
          if (!flgVirtual)
          {
            pos_fast += reststepfast;
            set_DACXY(portfast, pos_fast);
          } 
          else { pos_fast += reststepfast; }
          sleep_us(conf_.delayF);
        }
//******************************************************************************
         sleep_us(conf_.pause);    // 50 CONST 50ms wait for start get data
//******************************************************************************     

        if (!flgVirtual)
        {
          getValuesFromAdc();
          vector_data.emplace_back(ZMaxValue-(int16_t) spiBuf[ZPin]);  // считать Z из АЦП
        }
        else
        {
          vector_data.emplace_back(int16_t(10000.0 * (sin(M_PI * j * 0.1) + sin(M_PI * i * 0.1)))); 
        }
      } //j

      for (uint32_t j = 0; j < stepfastline * nfastline; ++j) // возврат в начальную точку линии
      {
        if (!flgVirtual)
        {
          pos_fast -= conf_.diskretinstep;
          set_DACXY(portfast, pos_fast);
        } 
        else { pos_fast -= conf_.diskretinstep; }
        sleep_us(conf_.delayB);
      }
      if (reststepfast != 0)// добирание остатка
      {
        if (!flgVirtual)
        {
          pos_fast -= reststepfast;
          set_DACXY(portfast, pos_fast);
        }
        else { pos_fast -= reststepfast; }

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
          } 
          else { pos_slow += conf_.diskretinstep; }
          sleep_us(conf_.delayF);
        }
        if (reststepslow != 0)
        {
          if (!flgVirtual) 
          {
            pos_slow -= reststepslow;
            set_DACXY(portslow, pos_slow);
          }
          else { pos_slow -= reststepslow; }
          sleep_us(conf_.delayF);
        }
      }
    } //i
    sendStrData("debug end fastscan");
    sendStrData("code56",vector_data,100,true);
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
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData("end");
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
  debugdata.emplace_back(pos_.x);
  debugdata.emplace_back(pos_.y);
  sendStrData("code18",debugdata,100,true);
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
  debugdata.emplace_back(pointX0Y0.x);
  debugdata.emplace_back(pointX0Y0.y);
  debugdata.emplace_back(delay);
  debugdata.emplace_back(pos_.x);
  debugdata.emplace_back(pos_.y);
  sendStrData("debug moveto parameters",debugdata,200,true);

  move_to(pointX0Y0, delay);

  sleep_ms(200);
  sendStrData("stopped");
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData("end");
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
  }
  else
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
  retract();  //втянуть сканер
  sleep_ms(50);
  if (!flgVirtual) linearDriver.activate(lid_name, f, p, std::abs(n), dir);
  protract();  //вытянуть сканер
 } 
  sleep_ms(1000);
  debugdata.emplace_back(n);
  debugdata.emplace_back(dir);
  sendStrData("debug autorising done ",debugdata,100,true);
}
void Scanner::positioningXYZ(std::vector<int32_t> &vector)
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
   for (int j = 0; j <= 7; ++j)
   {
     debugdata.emplace_back(vector[j]);
    }
   sendStrData("debug parameters pos update",debugdata,100,true);

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
        for (int j = 1; j <= 3; ++j)
        {
          debugdata.emplace_back(vector[j]);
        }
        sendStrData("debug parameters update",debugdata,100,true);
      }
      status = none;
      if (!flgVirtual) //add mf
      {
        linearDriver.activate(lid_name, f, p, std::abs(ln), ldir);
      } 
      else  {    }
      debugdata.emplace_back(status);
      debugdata.emplace_back(ZValue);
      debugdata.emplace_back(SignalValue);
      sendStrData("code"+ std::to_string(lid_name) ,debugdata,100,true);
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
       for (int j = 1; j <= 3; ++j)
       {
         debugdata.emplace_back(vector[j]);
       }
       sendStrData("debug parameters update",debugdata,100,true);
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
        if (ldir == 1) { ZValue -= ln; }
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
      debugdata.emplace_back(status);
      debugdata.emplace_back(ZValue);
      debugdata.emplace_back(SignalValue);
      sendStrData("code"+ std::to_string(lid_name) ,debugdata,100,true);
    }
  }
   STOP=false;
   debugdata.emplace_back(status);
   debugdata.emplace_back(ZValue);
   debugdata.emplace_back(SignalValue);
   sendStrData("code"+ std::to_string(lid_name) ,debugdata,100,true);
   sendStrData("stopped");
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData("end");
  dark();
}
/*
 static int16_t ZMove( int16_t Z0, int16_t steps, int16_t stepsize, uint16_t delay )   // stepsize=+-1  sign  -> dir 
	{
	  int16_t Zt;
	  Zt =Z0;
	  for (int16_t j=0; j< steps; j++)
	  {
      if (stepsize>0)  //вытягивание 
      {
        if (Zt>=(maxint16_t-stepsize)) { Zt=maxint16_t;}
      }
      else
      {
        if (Zt<=(minint16_t-stepsize)) { Zt=minint16_t;}
      } 
       Zt=Zt+stepsize;  
      if (!flgVirtual) set_DACZ(0,Zt);    // - physical - 0
      for(int16_t k=0; k < delay; k++) { }// задержка в каждом дискрете
	  }
    return(Zt);
	}
*/
static int16_t ZMove( int16_t Z0, int16_t steps, int16_t stepsize, uint16_t delay )   // stepsize=+-1  sign  -> dir 
	{
	  int16_t Zt;
	  Zt =Z0;
	  for (int16_t j=0; j< steps; j++)
	  {
      if (stepsize>0)  //вытягивание 
      {
        if (Zt>=(maxint16_t-stepsize)) { Zt=maxint16_t;}
      }
      else
      {
        if (Zt<=(minint16_t-stepsize)) { Zt=minint16_t;}
      } 
       Zt=Zt+stepsize;  
      if (!flgVirtual) set_DACZ(0,Zt);    // - physical - 0
      for(int16_t k=0; k < delay; k++) { }// задержка в каждом дискрете
	  }
    return(Zt);
	}
void Scanner::spectroscopyAIZ(std::vector<int32_t> &vector) // спектроскопия Ampl-Z
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

//  Z<=0 сканнер втянут
//  32767 - вытянут
// const int16_t SFM=0;
// const int16_t STM=1;
// const int16_t SICMDC=3;      
 int16_t   NPoints=(int16_t )vector[1]; // n точек
 int16_t    ZStart=(int16_t )vector[2]; // отход на  abs(ZStart)
 int16_t     ZStep=(int16_t )vector[3]; // ZStep>0
 int16_t Threshold=(int16_t )vector[4]; // Threshold
 int16_t     Delay=(int16_t )vector[5]; // delay
 int16_t   flgModa=(int16_t )vector[6]; // flgmode stm,sfm;

 int16_t SignalValue;
 int16_t dir,dlt,deltaZ;

 int16_t MicrostepDelay=3;

  for (int j = 0; j <= 6; ++j)
  {
    debugdata.emplace_back(vector[j]);
  }
  sendStrData("debug AI_Z parameters",debugdata,100,true);
 /*
   if (!flgVirtual) 
   { 
    auto ptr = getValuesFromAdc();
    dacZ0=(int16_t)ptr[ZPin];
    dacZ=dacZ0;
   }
   else
   {
    dacZ0=(int16_t)26000;
    dacZ=dacZ0;
   }
   */
 //start
  SignalValue=0;
  int16_t k=0;
  vectorA_Z.clear();
////////////////////////////////////////
 if(!flgVirtual)  freezeLOOP(200);
 //////////////////////////////////////
  sleep_ms(200);      
  
  deltaZ = ZMove(0, abs(ZStart), 1, MicrostepDelay ); // отвод в начальную точку - втягивание
    
 for(int16_t i=0; i<NPoints; i++)     //сближение
  {
    sleep_ms(Delay);  
   if (!flgVirtual) 
   {   auto ptr = getValuesFromAdc();   
        switch (flgModa)
    {
     case SFM:     { SignalValue=(int16_t)ptr[AmplPin]; break;}  
     case STM:
     case SICMDC:  { SignalValue=(int16_t)ptr[IPin];    break;}  
    }
   }
   else
   {
           switch (flgModa)
    {
     case SFM:     { SignalValue=-deltaZ;/*ZMaxValue-dacZ;*/ break;}  
     case STM:   
     case SICMDC:  { SignalValue=-deltaZ;/*ZMaxValue-dacZ*/ break;}  
    } 

   }
     vectorA_Z.emplace_back(SignalValue); 
     vectorA_Z.emplace_back(-deltaZ);
     vectorA_Z.emplace_back(1);
     k+=3;       	
   if (flgModa==STM) 
   {
    if ((abs(SignalValue)>abs(Threshold)) and (i<=(NPoints-1)))
     {
       break;
     }
   };
   if  ((flgModa==SICMDC) || (flgModa==SFM) )
   {
     if (abs(SignalValue)<abs(Threshold) and (i<=(NPoints-1)))
     {
      break; 
     }
   };
    deltaZ = ZMove(deltaZ, ZStep, -1, MicrostepDelay);
 }  // for    i
    NPoints= k / 3;
    sleep_ms(300);

  for(int16_t i=NPoints; i>=1; i--)  // отвод
  {
    sleep_ms(Delay); 

   if (!flgVirtual)
   {
      auto ptr = getValuesFromAdc();
      switch (flgModa)
    {
     case SFM:    { SignalValue=(int16_t)ptr[AmplPin]; break;}  
     case STM: 
     case SICMDC: { SignalValue=(int16_t)ptr[IPin];    break;}  
    }
   }
   else
   {
      switch (flgModa)
    {
     case SFM:    { SignalValue=-deltaZ+100; break;}  
     case STM:    
     case SICMDC: { SignalValue=-deltaZ+100; break;}  
    }
   }
     vectorA_Z.emplace_back(SignalValue);
     vectorA_Z.emplace_back(-deltaZ);
     vectorA_Z.emplace_back(-1);
     deltaZ = ZMove( deltaZ, ZStep, 1, MicrostepDelay);
  }
 //move to start point
  sleep_ms(300);
  dlt=abs(deltaZ);
  if (deltaZ>0) dir=-1;
  else          dir=1;
  deltaZ = ZMove(deltaZ, dlt, dir, MicrostepDelay );
  sendStrData("code66",vectorA_Z,100,true);  
 /////////////////////////////////////////  
 // разморозка состояния pid
    if(!flgVirtual) 
    {
     unfreezeLOOP(500); 
    }
 //////////////////////////////////////////
    int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData("end");
}

void Scanner::spectroscopyIV(std::vector<int32_t> &vector)
{
    int i,j;
		int16_t  UBackup;
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
    UBackup         = (int16_t) vector[7]; // V текущее значение напряжения 
//start
 for (int j = 0; j <= 6; ++j)
  {
    debugdata.emplace_back(vector[j]);
  }
  sendStrData("debug I_V parameters",debugdata,100,true);
///////////////////////////////////////////////////  
  if(!flgVirtual)  freezeLOOP(200);
////////////////////////////////////////////////////
  sleep_ms(300);
 // установка начального значения напряжения
      int16_t kk;
      int32_t dlt;
      int16_t nstep;
      int16_t rest;    
      start_step=100;
      dacU=UBackup;//+ShiftDAC; //240206
      UStart=UStart;//+ShiftDAC;
      step=-start_step;

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
      sendStrData("code65",vectorI_V,100,true);  
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
   if (!flgVirtual) set_Bias(1,dacU);  
   sleep_ms(10);
   dacU+=step;
  }
    dacU+=rest;
  if (!flgVirtual) set_Bias(1,dacU);  
    sleep_ms(10);
  if (!flgVirtual) set_Bias(1,UBackup);  //240206
    sleep_ms(10);
///////////////////////////////////////////////
   if(!flgVirtual)  unfreezeLOOP(500);
/////////////////////////////////////////////  
   int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData("end");  
}

void Scanner::approacphm(std::vector<int32_t> &vector) //uint16_t
{
  const int none = 30;
  const int ok = 3;
  const int touch = 2;
  const int stopdone = 1;
  uint16_t ZMaxValue = 32767;

  int16_t  SET_POINT;
  int16_t  GATE_Z_MAX, GATE_Z_MIN;
  int16_t  freq, scv;//
  int16_t  GAIN, NSTEPS;
  uint16_t INTDELAY, SCANNERDECAY;
  int16_t  flgDev;
  int32_t  Bias;
  // SET VALUE FROM RX_CORE
  SET_POINT      =(int16_t) vector[1]; // set point
  GATE_Z_MAX     =(int16_t) vector[2]; // max
  GATE_Z_MIN     =(int16_t) vector[3]; // min
  NSTEPS         =(int16_t) vector[4]; // steps 
  INTDELAY       =(uint16_t)vector[5]; // initdelay
  GAIN           =(int16_t) vector[6]; // gain
  SCANNERDECAY   =(uint16_t)vector[7]; // scannerDelay 
  freq           =(int16_t) vector[8]; // freq
  scv            =(int16_t) vector[9]; // scv
  flgDev         =(int16_t) vector[10];//  0= SFM, 1=STM ;SICMAC-2; SICMDC-3;  device type
  Bias           =(int16_t) vector[11];// Voltage need for STM,SICM
 //need to add channel Bias ????
 //need to add channel SetPoint ????

  for (size_t j = 0; j < 12; j++)     // send info
  {
    debugdata.emplace_back(vector[j]);
  } 
  sendStrData( "debug approach parameters 1 ",debugdata,100,true);
  set_SetPoint(0,SET_POINT); 
  if (flgDev!=SFM) set_Bias(1,Bias);  
  set_GainPID(GAIN);

  if (!flgVirtual)
  {
    uint16_t *ptr = getValuesFromAdc(); 
    ZValue = (int16_t) ptr[ZPin];
    switch (flgDev)
   {
      case SFM: 
     {
      SignalValue = (int16_t) ptr[AmplPin];
      break;  
     } 
      case STM:
   case SICMDC:  
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

  sendStrData( "code75",buf_status,100,false);

  while (true)
  { 
    sleep_ms(INTDELAY);
    buf_status[0] = none;
    if (STOP)
    {
      STOP=false;
      buf_status[0] = stopdone;
      buf_status[1] = ZValue;
      buf_status[2] = SignalValue;
      sendStrData("stopped");
    //  sleep_ms(200); //24/02/02
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
     
     // if (flgDev!=SFM) set_Bias(1,Bias);  240211
      set_SetPoint(0,SET_POINT); 
      set_GainPID(GAIN);
      sleep_ms(100);  // need for virtual для разделение afc
      for (int j = 1; j <= 7; ++j)
      {
        debugdata.emplace_back(vector[j]);
      }
      sendStrData("debug parameters update",debugdata,200,true);
    }
 
    if (!flgVirtual)
    {
      auto ptr = getValuesFromAdc();
      ZValue = (int16_t) ptr[ZPin];
     switch (flgDev)
     {
      case SFM: 
      {
       SignalValue = (int16_t) ptr[AmplPin];
       break;  
      } 
         case STM:
      case SICMDC:  
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
      }
      else
      {
        if ((ZMaxValue - ZValue) > 0) { ZValue += 500; }
                                 else { ZValue = maxint16_t; }
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
        if (buf_status[0] == 3)  { break; }
      }
    } //NSTEPS>0
    if (NSTEPS < 0)
    {

    }
    sendStrData( "code75",buf_status,100,false);
    if (!flgVirtual)
    {
      retract();  //втянуть сканнер
      sleep_ms(SCANNERDECAY);
      linearDriver.activate(99, freq, scv, std::abs(NSTEPS), NSTEPS > 0);
      protract(); //вытянуть
    }
  }
  sendStrData( "code75",buf_status,100,false);
  if (!flgVirtual)
  {
    protract();//вытянуть
    sleep_ms(INTDELAY);
  }
 //  sendStrData("stopped");
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData("end");
}

void Scanner::start_frqscan()
{
  static uint16_t inBuf[5]; // n, start_freq, step, channel, delay
  int16_t signalvalue;
  int16_t res_freq = 10000;
  int16_t a = 10000;
  int16_t scan_index = 0;
  int16_t current_freq = 0;
  int32_t current_channel;
  for (int j = 0; j < 5; ++j)
  {
    inBuf[j] = vector[1 + j];
    debugdata.emplace_back(inBuf[j]);
  }
  debugdata.emplace_back(flgVirtual);
  debugdata.emplace_back(AmplPin);
  debugdata.emplace_back(ZPin);
  sendStrData("debug frq scan parameters ",debugdata,100,true);
  
  current_channel = inBuf[3] - 1;  

  std::vector<int32_t> data;

  while ((scan_index++ < inBuf[0]))
  {
    if (!flgVirtual)
    {
      set_Freq(inBuf[1]);
      sleep_ms(inBuf[4]);
      data.emplace_back((int32_t)inBuf[1]);
      data.emplace_back((int32_t)getValuesFromAdc()[current_channel]);
    }
    else
    {
      current_freq = inBuf[1];
      sleep_ms(inBuf[4]);
      signalvalue = (int16_t) std::round(a * (pow(M_E, -pow((current_freq - res_freq), 2) / 1000000))); //231126
      data.emplace_back((int32_t)current_freq);
      data.emplace_back((int32_t)signalvalue);
    }
    sleep_ms(10);
    inBuf[1] += inBuf[2];
  }
  sendStrData("code25",data,100,true);
  //current_channel = -1;
  int16_t count = 0;
  while ((!TheadDone) || (count<20) )//ожидание ответа ПК для синхронизации
  {
    sleep_ms(100);
    count++;
  } 
  TheadDone = false;
  sendStrData("end");
}

