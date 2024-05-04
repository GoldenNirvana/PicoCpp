#include "device_variables.hpp"

Spi            spi;
LinearDriver   linearDriver;
Decoder        decoder(4, 5, 6);  // 4,5,6  ?
ConfigHardWare confighardwarev0({2,1,3,2,4,1,16,7,10,17,PICO_DEFAULT_LED_PIN,23,11,12,12,14,15,26,27});
ConfigHardWare confighardwarev1({2,1,3,2,4,1,16,7,10,17,PICO_DEFAULT_LED_PIN,23,11,12,12,14,15,26,27});
ConfigLinearDrive configlineardrivev0({18,19,20,21,22,28});
ConfigLinearDrive configlineardrivev1({18,19,20,21,22,28});