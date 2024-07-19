#include "rx_core.hpp"

#include <iostream>
#include <iomanip>
#include <pico/bootrom.h>
#include "pico/mutex.h"

#include "rx_utils/parser.hpp"
#include "../utilities/base_types/Spi.hpp"
#include "../loop/common_data/common_variables.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "../utilities/debug_logger.hpp"

//#warning DO NOT SLEEP IN THIS FUNC !!!

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
    if (vector.size()!=0)
   {
    switch (vector[0])
    { 
     /* case VirtualCmd : //флаг симуляции работы микроконтроллера      
        flgVirtual=(bool)vector[1];
        afc.clear();
        afc = code+std::to_string(DEBUG)+" virtual "+ std::to_string(vector[1]);
        afc +=endln;//"\n";
        std::cout << afc;
        afc.clear();
        sleep_ms(100); 
        break;
     */   
      case DebugLevelCmd: // флаг вывода отладочной информации debug level =2;  =3 запрет вывода!
        flgDebugLevel=vector[1];
        break;    
      case DebugCmd: // флаг вывода отладочной информации  =1, нет =0
        flgDebug=(bool)(vector[1]);
        afc.clear();
        afc = code+std::to_string(DEBUG)+"debug Set Debug "+ std::to_string(vector[1]);
        afc +=endln;//"\n";
        std::cout << afc;
        afc.clear();
        sleep_ms(100); 
        break;
      case SetUseCritialSectAlgCode: // флаг использовать Сritical_section
        flgСritical_section=(bool)vector[1]; 
        break;
  //***************************************  
      case USEPLDCmd:
      // flgUseFPGA=true;    
       break;       
      case ADC_GET_VALUECmd:            
        ADC_GET_VALUE = true;// прочитатать сигналы АЦП      
        break;
      case TheadDoneCmd:
        TheadDone = true;
        break;
      case DRAWDONECmd: 
        DrawDone = true;
        break;  
      case STOPCmd:
        STOP=true; // stop the active algorithm 
        break; 
      default: 
      {
         if (vector[0]>=0 && vector[0]<100)  {ALGCODE=(int16_t)vector[0]; }
                                       else ALGCODE=0;
        break;
      }  
     }   
     continue;
   } 
   if (vupdateparams.size()!=0)  
   {
    if (vupdateparams[0]==CONFIG_UPDATECmd)
    {
      CONFIG_UPDATE = true;
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
  std::cout << endln;//"\n";
 }
} 
void RX_core::serialPrint2Buffer(const uint16_t *const buf)
{
 // uint64_t a = time_us_64();
  std::cout <<code+ "12,"<< std::to_string(buf[0]) << ','<<std::to_string( buf[1]) <<"\n";
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
  std::cout << endln;//"\n";
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
  Parser parser(s.data(), separator);
  vectorSize = parser.parseInts(vec);
}
void RX_core::parse(std::vector<int32_t> &vec,std::vector<int32_t> &vparams)
{
  std::string s;
  getline(std::cin, s);
  // todo mb add const_cast
  Parser parser(s.data(), separator);
  vectorSize = parser.parseInts(vec,vparams);
}

