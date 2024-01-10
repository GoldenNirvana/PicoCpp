#include "rx_core.hpp"

#include <iostream>
#include <iomanip>
#include <pico/bootrom.h>
#include "pico/mutex.h"
#include "rx_utils/parser.hpp"
#include "../utilities/base_types/Spi.hpp"
#include "../loop/common_data/common_variables.hpp"
#include "../utilities/peripheral_functions.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "../utilities/debug_logger.hpp"

#warning DO NOT SLEEP IN THIS FUNC !!!

void RX_core::comReceiveISR(uint a, uint32_t b)
{
  if (ADC_IS_READY_TO_READ)
  {
  //  logger("ADC read recursion\n");
    return;
  }
  decoder.activePort(0);
  Spi::setProperties(16, 1, 0);
  spi_read16_blocking(spi_default, 0, spiBuf, 8);
  if (Z_STATE) 
  {
    Z_STATE = false;
    if (!flgVirtual) serialPrintBuffer(spiBuf, 8);
    return;
  } 
  ADC_IS_READY_TO_READ = true;
}
void RX_core::launchOnCore1()
{
  while (true)
  {
    parse(vector); // парсинг входящих данных из ПК
    switch (vector[0])
    {
      case 1:
        AD9833_SENDER = true;
        break;
      case 5:
        AD8400_SENDER = true;
        break;
      case 6:
        ADC_ENABLE_DISABLE = true;
        break;
      case 11:
        ADC_RESET = true;
        break;
      case 12:
        ADC_READ = true;
        break;
      case 13: 
        if (vector[1]==1)
        {
           ZPin=0; 
           AmplPin=1; 
           IPin=2;
        }
        else
        {
           ZPin=1; 
           AmplPin=0; 
           IPin=2;
        }
       break;
   //*************************************** 
      case 14: //флаг симуляции работы микрокотроллера      
        flgVirtual =(bool)vector[1];
        break;
      case 15: // флаг вывода отладочной инофрмации debug level =2;  =3 запрет вывода!
        flgDebugLevel =vector[1];
        break;
  //***************************************    
      case 16: //изменить значение усиления амплитуды раскачки зонда
        SET_AMPLMOD_GAIN=true;
        break;  
    /*  case 17: // изменить значение усиления ПИД
        SET_PID_GAIN=true; 
        break;
    */    
      case 18: // получить текущее знание координат сканера 
        GET_CURRENTX0Y0=true; 
        break; 
      case 19:
        SET_BIAS=true;
        break;    
      case 21:
        AD5664 = true;
        break;
      case 22:
        SET_SETPOINT=true;// управление опорой и напряжением  
        break;
      case 23:
        InitDAC_BIAS_SET_POINT=true;  //иницирование управлением опорой и напряжением     
        break;
      case 24:            
        ADC_GET_VALUE = true;// прочитатать сигналы АЦП      
        break;
      case 25:
        RESONANCE = true;
        break;
      case 27:
        InitDAC_XY=true;; // иницирование  управлением сканера по X,Y 
        break;
      case 28: // mf  
        TheadDone = true;
        break;
      case 29:
        SET_XY=true;// управление сканер X,Y  
        break;
      case 30:
        FREQ_SET = true;
        break;
      case 40:
        SET_AMPLMOD_GAIN= true;
        break;
      case 50:
        SCANNING = true;
        break;
      case 51:
        MOVE_TOX0Y0 = true;
        break;
  /*      
       case 53:
        SCANNING = true;
        break;
  */      
      case 55:
        SCAN_CONFIG_UPDATE = true;
        break;
      case 56:
        FASTSCANNING =true;
        break;
      case 60: // изменить значение усиления ПИД
        SET_PID_GAIN =true;
        break;
      case 61:  // управление ПИД - втянуть-вытянуть
        SCANNER_RETRACT_PROTRACT = true;
        break;
      case 65:  // спектроскопия  I-V
        SPECTROSOPY_IV = true;
        break;
      case 66:  // спектроскопия  A-Z
        SPECTROSOPY_AZ = true;
        break;  
      case 70:
        STOP=true; //stopAll(); stop algorithm 
        break;
      case 75: //сближение зонда и образца
        APPROACH = true;
        break;
      case 76:// изменение параметров сближения
        APPROACH_CONFIG_UPDATE = true;
        break;
      case 80:
        LID_MOVE_UNTIL_STOP = true; 
        break;
      case 82:// изменение параметров позиционирования ZYX
        POSXYZ_CONFIG_UPDATE = true;
        break;
      case 84:
        LID_MOVE_TOZ0 = true; 
        break;  
      case 90 ... 99:
        LID = true;
        break;
      default:
        activateError();
    }
  }
}

void RX_core::serialPrintBuffer(const uint16_t *const buf, int len)
{
 if (flgDebugLevel<=DEBUG_LEVEL)
 { 
  uint64_t a = time_us_64();
  std::cout << "[" << std::fixed << std::setfill('0') << std::setw(15) << std::right << a << "_u64]     ";
  std::cout << std::resetiosflags(std::ios_base::right);
  std::cout << std::resetiosflags(std::ios_base::fixed);
  for (int i = 0; i < len; ++i)
  {
    std::cout << buf[i] << ' ';
  }
  std::cout << "\n";
 }
} 
void RX_core::serialPrint2Buffer(const uint16_t *const buf)
{
 // uint64_t a = time_us_64();
  std::cout << "code12,"<< std::to_string(buf[0]) << ','<<std::to_string( buf[1]) <<"\n";
}

void RX_core::serialPrintBuffer(const uint8_t *const buf, int len)
{
 if (flgDebugLevel<=DEBUG_LEVEL)
 {
  uint32_t x = time_us_64();
  std::cout << "[" << std::fixed << std::setfill('0') << std::setw(15) << std::right << x << "]     ";
  std::cout << std::resetiosflags(std::ios_base::right);
  std::cout << std::resetiosflags(std::ios_base::fixed);
  for (int i = 0; i < len; ++i)
  {
    std::cout << buf[i] << ' ';
  }
  std::cout << "\n";
 }
}

void RX_core::parse(int32_t *vec)
{
  std::string s;
  getline(std::cin, s);
  // todo mb add const_cast
  Parser parser(s.data(), ',');
  for (int c = 0; c < 16; c++) //????
  {
    vec[c] = -1;
  }
  vectorSize = parser.parseInts(vec);
}



