#include "rx_core.hpp"

#include <iostream>
#include <pico/bootrom.h>
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
    logger("ADC read recursion\n");
    return;
  }
  decoder.activePort(0);
  Spi::setProperties(16, 1, 0);
  spi_read16_blocking(spi_default, 0, spiBuf, 8);
  if (Z_STATE)
  {
    ZValue = (int16_t) spiBuf[0]; //0 or 1
    SignalValue = (int16_t) spiBuf[1];
    Z_STATE = false;
    if (!flgVirtual) serialPrintBuffer(spiBuf, 8);
    return;
  }
  AD_7606_IS_READY_TO_READ = true;
}

void RX_core::launchOnCore1()
{
  while (true)
  {
    parse(vector);
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
        AD7606_READ = true;
        break;
      case 13:
        if (vector[1] == 1)
        {
          ZPin = 0;
          SignalPin = 1;
        } else
        {
          ZPin = 1;
          SignalPin = 0;
        }
        break;
      case 14: // set virtual device
        flgVirtual = (bool) vector[1];// !flgVirtual;
        break;
      case 15: // set debug level =2; if =3 cancel debug info!!
        flgDebugLevel = vector[1];
        break;
      case 17: // set PID GAIN!
        SET_PID_GAIN = true;
        break;
      case 18: //get current pointX0Y0 - pos_ 
        GET_CURRENTX0Y0 = true;
        break;
      case 21:
        AD5664 = true;
        break;
      case 22:
        DAC8563_SET_VOLTAGE_1 = true;
        break;
      case 23:
        DAC8563_INIT_1 = true;
        break;
      case 25:
        RESONANCE = true;
        break;
      case 26:
        RESONANCE_STOP = true;
        break;
      case 27:
        DAC8563_INIT_2 = true;
        break;
      case 28: // mf  
        TheadDone = true;
        break;
      case 29:
        DAC8563_SET_VOLTAGE_2 = true;
        break;
      case 30:
        FREQ_SET = true;
        break;
      case 40:
        AD8400_SET_GAIN = true;
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
        FASTSCANNING = true;
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
// if (flgDebugLevel<=DEBUG_LEVEL)
// {
//  uint64_t a = time_us_64();
//  std::cout << "[" << std::fixed << std::setfill('0') << std::setw(15) << std::right << a << "_u64]     ";
//  std::cout << std::resetiosflags(std::ios_base::right);
//  std::cout << std::resetiosflags(std::ios_base::fixed);
//  for (int i = 0; i < len; ++i)
//  {
//    std::cout << buf[i] << ' ';
//  }
//  std::cout << "\n";
// }
}

void RX_core::parse(int32_t *vec)
{
  std::string s;
  getline(std::cin, s);
  Parser parser((int8_t *) s.data(), ',');
  for (int c = 0; c < 10; c++)
  {
    vec[c] = -1;
  }
}



