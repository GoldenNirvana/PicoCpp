#include "device_variables.hpp"

Spi               spi;
//LinearDriver   *linearDriver;
Decoder           decoder(4, 5, 6);  // 4,5,6  ? //250506

ConfigHardWare    confighardwarev0({2,1,3,2,4,1,16,7,10,17,PICO_DEFAULT_LED_PIN,23,13,14,15,26,27});
ConfigHardWare    confighardwarev1({2,1,3,2,4,1,16,7,10,17,PICO_DEFAULT_LED_PIN,23,13,14,15,26,27,28,11,12,8,28});
 /* add new ports
  uint8_t ModulateUPort;    //28  вкл модуляцию U
  uint8_t SD_1Port;         //11 порты  настройки СД
  uint8_t SD_2Port;         //12  порты  настройки СД
  uint8_t SignLoopPort;     // 8 знак ПИД
  uint8_t Interator_InPort; //13 выбор вход сигнала на ПИД из Сд или ПТН(I)
*/
ConfigLinearDrive configlineardrivev0({18,19,20,21,22,28});
ConfigLinearDriveNew configlineardriveNew({18,19,20,21,22});