#ifndef PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#define PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#include <pico/critical_section.h>

struct ConfigHardWare
{
  uint8_t DACBSPT;// DAC8563
  uint8_t DACXY;  // DAC8563
  uint8_t DAC;    //dac8563_3
  uint8_t BUSY;
  uint8_t CONV;
  uint8_t DEC;
  uint8_t ResetPort;  
  uint8_t LEDPort;
  uint8_t RDBPort; 
  uint8_t IO1_0;
  uint8_t IO1_1;
  uint8_t IO2_0;
  uint8_t IO2_1;
  uint8_t IO2_2;
  uint8_t IO3_0;
  uint8_t IO3_1; 
};
//extern Scanner *scanner;
extern ConfigHardWare confighardwarev0;
extern ConfigHardWare confighardwarev1;
#endif //PICO_EXAMPLES_DEVICE_VARIABLES_HPP