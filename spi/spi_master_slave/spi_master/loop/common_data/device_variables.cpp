#include "device_variables.hpp"

Spi      spi;
LinearDriver   linearDriver;
Decoder        decoder(4, 5, 6);  // 4,5,6  ?
ConfigHardWare confighardwarev0({1,2,1,16,7,10,17,PICO_DEFAULT_LED_PIN,23,11,12,12,14,15,26,27});
ConfigHardWare confighardwarev1({1,2,1,16,7,10,17,PICO_DEFAULT_LED_PIN,23,11,12,12,14,15,26,27});
/*
uint8_t DACBSPT;// DAC8563_1  BIAS SetPoint
  uint8_t DACXY;  // DAC8563_2  XY
  uint8_t DAC;    // dac8563_3  Z
InputPort  busy(16); // FIXME TEMP!!!
OutputPort conv(7);
OutputPort dec(10);
OutputPort resetPort(17); // FIXME TEMP
OutputPort ledPort(PICO_DEFAULT_LED_PIN);
OutputPort rdbLed(23);
OutputPort io1_0(11); 
OutputPort io1_1(12);
OutputPort io2_0(13);
OutputPort io2_1(14);
OutputPort io2_2(15);
OutputPort io3_0(26); //вытянуть сканнер
OutputPort io3_1(27); //втянуть сканнер
*/