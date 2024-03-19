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
/*  if (Z_STATE) 
  {
    Z_STATE = false;
    if (!flgVirtual) serialPrintBuffer(spiBuf, 8);
    return;
  } 
*/  
  ADC_IS_READY_TO_READ = true;
}
void RX_core::launchOnCore1()
{
  
   while (true)
  {
    parse(vector,vupdateparams); //wait for data ! парсинг входящих данных из ПК 
 //   flgParamsUpdated=false;
//    while (not flgParamsUpdated;) sleep_ms(100);

   if (vector.size()!=0)
   {
     if (flgСritical_section) critical_section_enter_blocking(&criticalSection); //added 24/03/11
    switch (vector[0])
    {
    ///////////////////////////// ??? 
      case 1:
        AD9833_SENDER = true;
        break;
      case 5:
        AD8400_SENDER = true;
        break;
      case 6:
        ADC_ENABLE_DISABLE = true;
        break;
     ///////////////////////////   
      case 11:
        ADC_RESET = true;
        break;
    /*  case 13: 
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
    */   
    /* 
       case 21:
        AD5664 = true;
        break;
     */   
  //*************************************** 
      case VirtualCmd : //флаг симуляции работы микрокотроллера      
        flgVirtual=(bool)vector[1];
        break;
      case DebugLevelCmd: // флаг вывода отладочной инофрмации debug level =2;  =3 запрет вывода!
        flgDebugLevel=vector[1];
        break;    
      case DebugCmd: // флаг вывода отладочной инофрмации debug level =2;  =3 запрет вывода!
        flgDebug=boolean(vector[1]);
        afc.clear();
        afc = "code"+std::to_string(DEBUG)+"debug Set Debug "+ std::to_string(flgDebug);
        afc += +"\n";
        std::cout << afc;
        afc.clear();
        sleep_ms(100); 
        break;
  //***************************************     
      case ADC_GET_VALUECmd:            
        ADC_GET_VALUE = true;// прочитатать сигналы АЦП      
        break;
      case TheadDoneCmd: // mf  
        TheadDone = true;
        break;
      case DRAWDONECmd: // mf  
        DrawDone = true;
        break;
  /*
      case CONFIG_UPDATECmd: //обновление параметров текущего активного алгоритма
        CONFIG_UPDATE = true;
        break;  
   */     
      case STOPCmd:
        STOP=true; //stopAll(); stop the active algorithm 
        break; 
      default: 
      {
   //    critical_section_enter_blocking(&criticalSection);
        if (vector[0]>=0 && vector[0]<100)  {ALGCODE=(int16_t)vector[0]; }
                                       else ALGCODE=0;
   //   critical_section_exit(&criticalSection);
        break;
      }  
     }   
     if (flgСritical_section) critical_section_exit(&criticalSection);
    
    continue;
   } 
   if (vupdateparams.size()!=0)  
   {
    if (vupdateparams[0]==CONFIG_UPDATECmd)
    {
      if (flgСritical_section) critical_section_enter_blocking(&criticalSection);
      CONFIG_UPDATE = true;
      if (flgСritical_section) critical_section_exit(&criticalSection);
    } 
   }
  }//while
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
/*
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
*/
void RX_core::parse(std::vector<int32_t> &vec)
{
  std::string s;
  getline(std::cin, s);
  // todo mb add const_cast
  Parser parser(s.data(), ',');
 // vec.clear();
  vectorSize = parser.parseInts(vec);
}
void RX_core::parse(std::vector<int32_t> &vec,std::vector<int32_t> &vparams)
{
  std::string s;
  getline(std::cin, s);
  // todo mb add const_cast
  Parser parser(s.data(), ',');
 // vec.clear();
  vectorSize = parser.parseInts(vec,vparams);
}

