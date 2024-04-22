#ifndef PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP
#define PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP
#include <vector>
#include "../devices/ad5664.hpp"
#include "../devices/DAC8563.hpp"
#include "../physical_devices/LinearDriver.hpp"
#include <ctime>
#include <cstdint>
#include <hardware/clocks.h>

class HARDWARE
{
// WARNING HARDCODED FUNCTIONS
private:
 DAC8563    *dac8563_1; // DAC BIAS,SetPoint
 DAC8563    *dac8563_2; // DAC X,Y
 DAC8563    *dac8563_3; // DAC Z
 InputPort  *busy; // FIXME TEMP!!!
 OutputPort *conv;
 OutputPort *dec;
 OutputPort *resetPort; // FIXME TEMP
 OutputPort *ledPort;
 OutputPort *rdbLed;
 OutputPort *io1_0; 
 OutputPort *io1_1;
 OutputPort *io2_0;
 OutputPort *io2_1;
 OutputPort *io2_2;
 OutputPort *io3_0; //вытянуть сканнер
 OutputPort *io3_1; //втянуть сканнер

 std::vector<OutputPort*> io_ports; 
 
 uint16_t *repeatTwoTimes(); 

 void get_result_from_adc();       // чтение АЦП
 
 void set_io_value(int, int);

 void set_clock_enable();
 
 void _delay_us(double __us);
 
 void activateGreen();

 void activateRed();

 void activateBlue();

public:

   HARDWARE();

  ~HARDWARE();

 void activateDark();

 void green();

 void blue();

 void red();
 
 void dark();

[[noreturn]] void activateError();

 void setDefaultSettings();

 void GetSOFTHARDWAREVersion();

 void set_Freq(uint32_t freq); //установка заданной частоты генератора

 void init_SPI(uint8_t port ,uint8_t v2 ,uint8_t v3, uint8_t v4); //инициирование SPI

 void init_DACSPB(uint8_t port); //инициирование ЦАП1  SetPoint,BIAS

 void init_DACXY(uint8_t port); //инициирование ЦАП2  XY

 void init_DACZ(uint8_t port); //инициирование ЦАП2  Z

 void set_Bias(int32_t Bias); //установка заданного значения напряжения

 void set_SetPoint(int32_t SetPoint);//установка заданной опроры для ПИД

 void set_GainApmlMod(uint8_t gain); // установить усиления 

 void set_GainPID(int gain);          //установить усиления ПИД

 void set_DACXY(uint8_t channel, uint16_t value); 

 void set_DACZ(int16_t value); 

 void set_DACZero();
 
 void move_scannerX(int x);

 void move_scannerY(int y);

 uint16_t *getValuesFromAdc();  // чтение АЦП

  void scanner_retract_protract(int port, int flg);
  
  void retract();       // втянуть сканер

  void retract(int16_t HeightJump); //втянуть на H

  void protract();      // втянуть сканер
 
 // void protract(uint16_t delay,int16_t DacZ0,int16_t HeightJump) ; //разморозить ПИД 
 
  void LOOP_freeze_unfreeze(int port, int flg);  
 
  void freezeLOOP(uint16_t delay);    // заморизить ПИД

  void unfreezeLOOP(uint16_t delay);  // разморизить ПИД

};
#endif
