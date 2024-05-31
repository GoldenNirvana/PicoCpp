#include "device_variables.hpp"

Spi                  spi;
Decoder              decoder(4, 5, 6);  
ConfigHardWare       confighardwarev0({2,1,3,2,4,1,16,7,10,17,PICO_DEFAULT_LED_PIN,23,11,12,13,14,15,26,27});
ConfigHardWareNew    confighardwarev1({2,1,3,2,4,1,16,7,10,17,PICO_DEFAULT_LED_PIN,23,13,14,15,26,27,28,11,12,8,13});
ConfigLinearDrive    configlineardrivev0({18,19,20,21,22,28});
ConfigLinearDriveNew configlineardrivev1({18,19,20,21,22});