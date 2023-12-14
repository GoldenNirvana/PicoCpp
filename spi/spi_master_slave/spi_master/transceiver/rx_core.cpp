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
  if (AD_7606_IS_READY_TO_READ)
  {
  //  logger("ADC read recursion\n");
    return;
  }
  decoder.activePort(0);
  Spi::setProperties(16, 1, 0);
  spi_read16_blocking(spi_default, 0, spiBuf, 8);
  if (Z_STATE)
  {
        ZValue =(int16_t)spiBuf[0]; //0 or 1
    SignalValue=(int16_t)spiBuf[1];
    Z_STATE = false;
    if (!flgVirtual) serialPrintBuffer(spiBuf, 8);
    return;
  } 
  AD_7606_IS_READY_TO_READ = true;
  /*
  if (is_already_scanning)
  { //231025
    if (!flgVirtual) {afc += std::to_string(current_freq) + ',' + std::to_string(spiBuf[current_channel]) + ',';}
    else { 
          current_freq=1000;
          afc += std::to_string(current_freq) + ',' + std::to_string(current_freq) + ',';
         }
  }
  else 
  if (AD7606_TRIG_GET_VALUE)
  {
    AD7606_TRIG_GET_VALUE = false;
    if (current_channel == -1)
    {
      std::cout << "error\n";
      return;
    }
    std::cout << "code24,"<<spiBuf[current_channel] << '\n'; // add "code,"  codevalue, value //add 231025
    current_channel = -1;
  } 
  else 
  if (AD7606_GET_ALL_VALUES)
  {
    AD7606_GET_ALL_VALUES = false;
    if (!flgVirtual) {serialPrintBuffer(spiBuf, 8);}//edited 231025
    else { serialPrint2Buffer(spiBuf);} 
    AD_7606_IS_READY_TO_READ = true;
  }
  */
}

void RX_core::launchOnCore1()
{
  while (true)
  {
//  vector[0] = 12;
//  std::cout << "Receive core\n";
//  mutex_enter_timeout_ms(&mut, 10);
//  std::cout << "Mutex captured by core1 << '\n";
//  PARSING
    parse(vector);
//  logger("command" + std::to_string(vector[0]) + '\n'); //231210
//  uart_puts(uart1, "String for uart");
    switch (vector[0])
    {
      case 1:
        AD9833_SENDER = true;
        break;
      case 5:
        AD8400_SENDER = true;
        break;
      case 6:
        AD7606_ENABLE_DISABLE = true;
        break;
      case 11:
        AD7606_RESET = true;
        break;
      case 12:
//          std::cout << "SetTrue\n";
        AD7606_READ = true;
        break;
 ///*************************************  
      case 13: //add MF 
        if (vector[1]==1)
        {
           ZPin=0; 
           AmplPin=1; 
        }
        else
        {
           ZPin=1; 
           AmplPin=0; 
        }
       break;
      case 14: //add MF set virtual device 
     //   red();      
        flgVirtual =(bool)vector[1];// !flgVirtual;
        break;
      case 15: //add mf set debug level =2; if =3 cancel debug info!!
         flgDebugLevel =vector[1];
        break;
      case 16: //add mf set gain ampl!!
        SET_AMPLMOD_GAIN=true;
        break;  
      case 17: //add mf set PID GAIN!    
        SET_PID_GAIN=true; 
        break;
      case 18: //get current pointX0Y0 - pos_ 
        GET_CURRENTX0Y0=true; 
        break; 
 //*************************************** 
      case 19:
        SETBIAS=true;
        break;    
      case 21:
        AD5664 = true;
        break;
      case 22:
        SET_SETPOINT=true;
        //DAC8563_SET_VOLTAGE_1 = true; // управление опорой и Bias
        break;
      case 23:
        DAC8563_INIT_1 = true;       // init  управление опорой и Bias
        break;
      case 24:// get signal value current signal or all signal ?
        AD7606_GET_VALUE = true;
        break;
      case 25:
        RESONANCE = true;
        break;
      case 26:
        RESONANCE_STOP = true;
        break;
      case 27:
        DAC8563_INIT_2 = true;    // init управление X,Y
        break;
      case 28: // mf  
        TheadDone = true;
        break;
      case 29:
        DAC8563_SET_VOLTAGE_2 = true; // управление X,Y
        break;
      case 30:
        FREQ_SET = true;
        break;
      case 40:
    //    AD8400_SET_GAIN = true;
        SET_AMPLMOD_GAIN= true;
        break;
      case 50:
        SCANNING = true;
    //    CONFIG_UPDATE = true;
        break;
      case 51:
        MOVE_TOX0Y0 = true;
        break;
      case 52:
        STOP_ALL = true;
        break;
      case 53:
        SCANNING = true;
        break;
      case 55:
        CONFIG_UPDATE = true;
        break;
      case 56:
        FASTSCANNING =true;
        break;
      case 60:
        SET_IO_VALUE = true;
        break;
      case 61:
        SET_ONE_IO_VALUE = true;
        break;
      case 70:
        stopAll();
        break;
      case 75: //approach
        APPROACH = true;
        break;
      case 76:// change parameters  approach
        APPROACH_CONFIG_UPDATE = true;
        break;
      case 80:
        LID_UNTIL_STOP = true; 
        break;
      case 82:// change parameters  positionXYZ
        POSXYZ_CONFIG_UPDATE = true;
        break;
      case 84:
        MOVE_TOZ0 = true; 
        break;  
      case 90 ... 99:
        LID = true;
        break;
      default:
        activateError();
    }
//        mutex_exit(&mut);
//        std::cout << "Mutex released by core1\n";
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
  for (int c = 0; c < 10; c++)
  {
    vec[c] = -1;
  }
  vectorSize = parser.parseInts(vec);
}



