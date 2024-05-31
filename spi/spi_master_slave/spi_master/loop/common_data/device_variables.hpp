#ifndef PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#define PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#include <pico.h>
#include "../../utilities/base_types/Spi.hpp"
#include "../../utilities/base_types/decoder.hpp"
//#include "../../physical_devices/LinearDriver.hpp"


struct ConfigHardWare
{
  uint8_t DACBiasSetPointPort;  //2 DAC8563_1  BIAS SetPoint
  uint8_t DACBiasSetPointMode;  //1 DAC8563_1  BIAS SetPoint
  uint8_t DACXYPort;    //3 DAC8563_2  XY
  uint8_t DACXYMode;    //2 DAC8563_2  XY
  uint8_t DACZPort;     //4 dac8563_3  Z
  uint8_t DACZMode;     //1 
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
struct ConfigHardWareNew
{
  uint8_t DACBiasSetPointPort;  //2 DAC8563_1  BIAS SetPoint
  uint8_t DACBiasSetPointMode;  //1 DAC8563_1  BIAS SetPoint
  uint8_t DACXYPort;    //3 DAC8563_2  XY
  uint8_t DACXYMode;    //2 DAC8563_2  XY
  uint8_t DACZPort;     //4 dac8563_3  Z
  uint8_t DACZMode;     //1 
  uint8_t BUSYPort;     //16
  uint8_t CONV;         //7
  uint8_t DEC;          //10
  uint8_t ResetPort;    //17
  uint8_t LEDPort;      //PICO_DEFAULT_LED_PIN
  uint8_t RDBPort;      //23
 // uint8_t IO1_0;        //11
 // uint8_t IO1_1;        //12
  uint8_t GainPID0;     //13
  uint8_t GainPID1;     //14
  uint8_t GainPID2;     //15
  uint8_t FreezePort;   //26 заморозить сканнер=1; разморозить =0
  uint8_t ProtractPort; //27 втянуть    сканнер=1; вытянуть    =0

  uint8_t ModulateUPort;   // вкл=1; выкд=0 модуляцию U  
  uint8_t SD_1Port;        // порты  настройки СД I_STM=1; 0 =др
  uint8_t SD_2Port;        // порты  настройки СД Cantilever=0; 1-Piezo
  uint8_t SignLoopPort;    // знак ПИД // 0=+ ; 1=-
  uint8_t Interator_InPort;// выбор вход сигнала на ПИД из1-SD; 0=ПТН(I) 
};

struct ConfigLinearDrive
{
  uint8_t XA_Port;     
  uint8_t XB_Port;   
  uint8_t YA_Port;    
  uint8_t YB_Port;     
  uint8_t ZA_Port;     
  uint8_t ZB_Port;     
};
struct ConfigLinearDriveNew
{
  uint8_t A_Port;     
  uint8_t B_Port;   
  uint8_t XTurn_on_Port;    
  uint8_t YTurn_on_Port;     
  uint8_t ZTurn_on_Port;        
};

extern Spi               spi;
//extern LinearDriver      *linearDriver;
extern Decoder           decoder;
extern ConfigHardWare       confighardwarev0;
extern ConfigHardWareNew    confighardwarev1;
extern ConfigLinearDrive    configlineardrivev0;
extern ConfigLinearDriveNew configlineardrivev1;
#endif //PICO_EXAMPLES_DEVICE_VARIABLES_HPP