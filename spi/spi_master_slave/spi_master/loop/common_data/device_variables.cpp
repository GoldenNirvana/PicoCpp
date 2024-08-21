#include "device_variables.hpp"

uint8_t FPGADELIM=0xAA;
uint8_t FPGACRCPAR=0xBB;
uint8_t FPGAREAD=0x00;
uint8_t FPGAWRITE=0x01;
uint8_t FPGAASC=0x80;

Spi                  spi;
Decoder              decoder(4, 5, 6);  
ConfigHardWare       confighardwarev0({2,1,3,2,4,1,16,7,10,17,PICO_DEFAULT_LED_PIN,23,11,12,13,14,15,26,27});
ConfigHardWareNew    confighardwarev1({2,1,3,2,4,1,16,7,10,17,PICO_DEFAULT_LED_PIN,23,13,14,15,26,27,28,11,12,8,13});
ConfigLinearDrive    configlineardrivev0({18,19,20,21,22,28});
ConfigLinearDriveNew configlineardrivev1({18,19,20,21,22});

FPGAAdress           arrModule_0(
                      {
                       0x08430000,//wbKx[0]
                       0x08430004,//wbKx[1]
                       0x08430008,//wbKx[2]
                       0x0843000C,//wbInMulKoef
                       0x08430010,//wbInShift
                       0x08430014,//wbOutMulKoef
                       0x08430018,//wbOutShift
                       0x0843001C,//wbSetpoint
                       0x08430020 //pidControl
                       });
