#ifndef PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#define PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#include <pico.h>
#include "../../utilities/base_types/Spi.hpp"
#include "../../physical_devices/LinearDriver.hpp"


struct ConfigHardWare
{
  uint8_t DACBSPTPort;// DAC8563_1  BIAS SetPoint
  uint8_t DACXYPort;  // DAC8563_2  XY
  uint8_t DACZPort;   // dac8563_3  Z
  uint8_t BUSYPort;
  uint8_t CONV;
  uint8_t DEC;
  uint8_t ResetPort;  
  uint8_t LEDPort;
  uint8_t RDBPort; 
  uint8_t IO1_0;
  uint8_t IO1_1;
  uint8_t GainPID0;
  uint8_t GainPID1;
  uint8_t GainPID2;
  uint8_t FreezePort; //заморозить сканнер=1; разморозить =0
  uint8_t ProtractPort; //втянуть    сканнер=1; вытянуть    =0
};
extern Spi            spi;
extern LinearDriver   linearDriver;
extern Decoder        decoder;
extern ConfigHardWare confighardwarev0;
extern ConfigHardWare confighardwarev1;
#endif //PICO_EXAMPLES_DEVICE_VARIABLES_HPP