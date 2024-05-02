#ifndef PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#define PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#include <pico.h>
#include "../../utilities/base_types/Spi.hpp"
#include "../../physical_devices/LinearDriver.hpp"


struct ConfigHardWare
{
  uint8_t DACBSPTPort;  //1 DAC8563_1  BIAS SetPoint
  uint8_t DACXYPort;    //2 DAC8563_2  XY
  uint8_t DACZPort;     //1 dac8563_3  Z
  uint8_t BUSYPort;     //16
  uint8_t CONV;         //7
  uint8_t DEC;          //10
  uint8_t ResetPort;    //17
  uint8_t LEDPort;      //PICO_DEFAULT_LED_PIN
  uint8_t RDBPort;      //23
  uint8_t IO1_0;        //11
  uint8_t IO1_1;        //12
  uint8_t GainPID0;     //13
  uint8_t GainPID1;     //14
  uint8_t GainPID2;     //15
  uint8_t FreezePort;   //26 заморозить сканнер=1; разморозить =0
  uint8_t ProtractPort; //27 втянуть    сканнер=1; вытянуть    =0
};
extern Spi            spi;
extern LinearDriver   linearDriver;
extern Decoder        decoder;
extern ConfigHardWare confighardwarev0;
extern ConfigHardWare confighardwarev1;
#endif //PICO_EXAMPLES_DEVICE_VARIABLES_HPP