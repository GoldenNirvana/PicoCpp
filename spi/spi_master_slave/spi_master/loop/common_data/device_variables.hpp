#ifndef PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#define PICO_EXAMPLES_DEVICE_VARIABLES_HPP
#pragma  pack(push, 1) //  240702 выравнивание структуры на границе byte
#include <pico.h>
#include "../../utilities/base_types/Spi.hpp"
#include "../../utilities/base_types/decoder.hpp"

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
//#define UART_TX_PIN 16 //240703
//#define UART_RX_PIN 17 //240703
//#define USB_UART_ID     uart1//240703
//#define USBUART_TX_PIN  8
//#define USBUART_RX_PIN  9
#define FPGAUART_TX_PIN 8 //!
#define FPGAUART_RX_PIN 9 //!
#define FPGA_UART_ID    uart1
#define FPGA_BAUD_RATE  115200

extern uint8_t FPGADELIM;
extern uint8_t FPGACRCPAR;
extern uint8_t FPGAREAD;
extern uint8_t FPGAREADADC;
extern uint8_t FPGAWRITE;
extern uint8_t FPGAASC;
extern uint32_t ZAdress;
extern uint32_t AmplAdress;
extern uint32_t IAdress;

struct ADCData
{
 int16_t Z;
 int16_t Signal;
};
extern ADCData adcdata;
struct ConfigHardWare //BB and BBFPGA
{
  uint8_t DACSetPointPort;  //2 DAC8563_1  BIAS SetPoint
  uint8_t DACBiasVPort;  //2 DAC8563_1  BIAS SetPoint
  uint8_t DACSetPointMode;  //1 DAC8563_1  BIAS SetPoint
  uint8_t DACBiasVMode;  //1 DAC8563_1  BIAS SetPoint
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
struct FPGAAdress
{
 uint32_t wbKx[3];
 uint32_t wbInMulKoef;
 uint32_t wbInShift;
 uint32_t wbOutMulKoef;
 uint32_t wbOutShift; //DACZ
 uint32_t wbSetpoint;
 uint32_t pidControl;
};
/*
Rx Frame format big-endian Offs:
  Size:      1       1       4        4         1         1
Fields: [ DELIM ] [ CMD ] [ ADDR ] [<DATA>] [CRC/PAR] [ DELIM ]
*/
struct FPGAWriteData
{
 uint8_t  delimbegin=FPGADELIM;
 uint8_t  cmd=FPGAWRITE;
 uint32_t addr;
 uint32_t data;
 uint8_t  crcpar=FPGACRCPAR;
 uint8_t  delimend=FPGADELIM;
 };
struct FPGAReadData
{
 uint8_t  delimbegin=FPGADELIM;
 uint8_t  cmd=FPGAREAD;
 uint32_t addr;
 uint8_t  crcpar=FPGACRCPAR;
 uint8_t  delimend=FPGADELIM;
 };
struct FPGAReadDataArray
{
 uint8_t  delimbegin=FPGADELIM;
 uint8_t  cmd=FPGAREADADC;
 uint8_t  crcpar=FPGACRCPAR;
 uint8_t  delimend=FPGADELIM;
 };
 struct FPGAAscData
{
 uint8_t  delimbegin=FPGADELIM;
 uint8_t  cmd=FPGAASC;
 uint32_t addr;
 uint8_t  crcpar=FPGACRCPAR;
 uint8_t  delimend=FPGADELIM;
 };
struct ConfigHardWareNew  //WB
{
  uint8_t DACSetPointPort;    //2 DAC8563_1  BIAS SetPoint
  uint8_t DACBiasVPort;    //2
  uint8_t DACBiasVMode;    //1 DAC8563_1  BIAS SetPoint
  uint8_t DACSetPointMode; //1 DAC8563_1  BIAS SetPoint
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

extern Spi                  spi;
//extern LinearDriver      *linearDriver;
extern Decoder              decoder;
extern ConfigHardWare       confighardwarev0;
extern ConfigHardWareNew    confighardwarev1;
extern ConfigLinearDrive    configlineardrivev0;
extern ConfigLinearDriveNew configlineardrivev1;
extern FPGAAdress           arrModule_0;
#endif //PICO_EXAMPLES_DEVICE_VARIABLES_HPP