#include "device_variables.hpp"

//ADCData adcdata;
uint8_t  FPGADELIM=0x0A; //0xAA?
uint8_t FPGACRCPAR=0xBB;
uint8_t   FPGAREAD=0x00;
uint8_t  FPGAWRITE=0x01;
uint8_t    FPGAASC=0x80;
uint8_t  FPGAREADADC=0x03; ///?????????????????   Согласовать
uint32_t ZAdress=0x08410004;
uint32_t AmplAdress=0x08410008;
uint32_t IAdress=0x0841000C;

Spi                  spi;
Decoder              decoder(4, 5, 6);  
ConfigHardWare       confighardwarev0
                      (
                       {                        
                        2, //DACSetPointPort
                        1, //DACSetPointMode
                        2, //DACBiasVPort
                        1, //DACBiasVMode
                        3, //DACXYPort
                        2, //DACXYMode
                        4, //DACZport
                        1, //DACZMode
                        16,//BUSYPort
                        7, //CONV
                        10,//DEC
                        17,//ResetPort
                        PICO_DEFAULT_LED_PIN,//LEDPort
                        23,//RDBPort
                        11,//IO1_0
                        12,//IO1_1
                        13,//GainPID0
                        14,//GainPID1
                        15,//GainPID2
                        26,//FreezePort  заморозить сканнер=1; разморозить =0
                        27 //ProtractPor  втянуть    сканнер=1; вытянуть    =0
                       }
                      );
ConfigHardWareNew    confighardwarev1
                      (
                       {
                        2,//SetPointPort 
                        1,//SetPointMode
                        2,//BiasVPort
                        1,//BiasVMode
                        3,//DACXYPort
                        2,//DACXYMode
                        4,//DACZPort
                        1,//DACZMode
                        16,//BUSYPort
                        7, //CONV
                        10,//DEC
                        17,// ResetPort
                        PICO_DEFAULT_LED_PIN,//LEDPort
                        23,//RDBPort
                        13,//GainPID0
                        14,//GainPID1
                        15,//GainPID2
                        26,//FreezePort   заморозить сканнер=1; разморозить =0
                        27,//ProtractPort втянуть    сканнер=1; вытянуть    =0
                        28,//ModulateUPort вкл=1; выкд=0 модуляцию U  
                        11,//SD_1Port Порты  настройки СД I_STM=1; 0 =др
                        12,//SD_2Port порты  настройки СД Cantilever=0; 1-Piezo
                        8, //SignLoopPort знак ПИД // 0=+ ; 1=-
                        13 //Interator_InPort выбор вход сигнала на ПИД из1-SD; 0=ПТН(I) 
                        }
                       );
ConfigLinearDrive    configlineardrivev0({18,19,20,21,22,28});
//ConfigLinearDriveNew configlineardrivev1({18,19,20,21,22}); //2241003
ConfigLinearDriveNew configlineardrivev1({18,19,22,21,20});

FPGAAdress           arrModule_0(
                      {
                       0x08430000,//wbKx[0]
                       0x08430004,//wbKx[1] //интегратор
                       0x08430008,//wbKx[2]
                       0x0843000C,//wbInMulKoef
                       0x08430010,//wbInShift
                       0x08430014,//wbOutMulKoef
                       0x08430018,//wbOutShift
                       0x0843001C,//wbSetpoint
                       0x08430020 //pidControl
                       });
