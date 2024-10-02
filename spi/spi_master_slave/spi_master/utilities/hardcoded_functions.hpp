#ifndef PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP
#define PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP
#include <vector>
#include <ctime>
#include <cstdint>
#include <iostream> //add
#include <hardware/clocks.h>

#include "../utilities/base_types/io_ports.h"
#include "../devices/ad5664.hpp"
#include "../devices/DAC8563.hpp"

#include "../physical_devices/LinearDriver.hpp"  //240505
#include "../loop/common_data/device_variables.hpp"
#include "../loop/common_data/common_variables.hpp"


class HARDWARE
{
// WARNING HARDCODED FUNCTIONS
private:
 DAC8563    *dacspt; // DAC BIAS,SetPoint
 DAC8563    *dacbv; // DAC BIAS,SetPoint
 DAC8563    *dacxy;   // DAC X,Y
 DAC8563    *dacz;    // DAC Z
 InputPort  *busyport;     // FIXME TEMP!!!
 OutputPort *conv;
 OutputPort *dec;
 OutputPort *resetport; // FIXME TEMP
 OutputPort *ledPort;
 OutputPort *rdbLed;
 OutputPort *io1_0; 
 OutputPort *io1_1;
 OutputPort *gainPID0;
 OutputPort *gainPID1;
 OutputPort *gainPID2;
 OutputPort *freezeport;       // заморозить/разморозить
 OutputPort *protractport;     // втянуть сканнер/вытянуть сканнер
 // add
 OutputPort *modulateuport;    // вкл модуляцию U
 OutputPort *i_stmport;        // порты  настройки СД читать modulalate signal I_STM
 OutputPort *sensorport;       // порты  настройки выбор сенсора
 OutputPort *signloopport;     // знак ПИД
 OutputPort *integrator_inport;// выбор вход сигнала вход. на ПИД из Сд или ПТН(I)

 uint16_t *repeatTwoTimes(); 

 void get_result_from_adc();       // чтение АЦП

 void set_clock_enable();
 
 void _delay_us(double __us);
 
 void activateGreen();

 void activateRed();

 void activateBlue();

 void WriteDataToFPGA(FPGAWriteData writedata);

 void AscResult(FPGAAscData ascdata, uint8_t* dst, size_t len);

public:
   LinearDriverBase  *linearDriver;

   HARDWARE(ConfigHardWare confighardware);

   HARDWARE(ConfigHardWareNew confighardware);

  ~HARDWARE();

 void activateDark();

 void green();

 void blue();

 void red();
 
 void dark();

[[noreturn]] void activateError();
 //инициирование ЦАП1  SetPoint,BIAS
 void setDefaultSettings( ConfigHardWare  confighardware);      //BB,BBFPGA

 void setDefaultSettings( ConfigHardWareNew  confighardware);   //WB
 
 void GetSOFTHARDWAREVersion();

 void set_Freq(uint32_t freq);    // установка заданной частоты генератора

 void setLoopSign(int8_t value);  // 0->+ ; 1-> -1

 void setSignal_In_Loop(int8_t value); // Ampl=1 ; I=0
 
 void useModulateI(int8_t value); // 1-> none; 0->use
 
 void setSensor(int8_t value);    // cantilever=1;  probe =0
 
 void setModulateU(int8_t value); // 0-> none; 1->use

 void init_commutation(uint8_t sensor ,uint8_t loopsign ,uint8_t signal_in_loop , uint8_t usemod_i,uint8_t usemod_u);
 
 void init_SPI(uint8_t port ,uint8_t v2 ,uint8_t v3, uint8_t v4); //инициирование SPI

 void init_DACSetPointBiasV(uint8_t spiport);  //инициирование ЦАП1  SetPoint,BIASV
 
 void init_DACSetPoint(uint8_t spiport);  

 void init_DACBiasV(uint8_t spiport);  

 void init_DACXY(uint8_t spiport);   //инициирование ЦАП2  XY

 void init_DACZ(uint8_t spiport);    //инициирование ЦАП2  Z

 void set_BiasV(int32_t BiasV);      //установка заданного значения напряжения

 void set_SetPoint(int32_t SetPoint);//установка заданной опроры для ПИД

 void set_GainApmlMod(uint8_t gain); //установить усиления модуляции амплитуды

 void set_GainPID(uint16_t gain);    //установить усиления ПИД

 void set_GainPID(uint32_t gain);    //установить усиления ПИД

 void set_GainPIDFPGA(uint32_t gain);    //установить усиления ПИД
 
 void set_DACXY(uint8_t channel, uint16_t value); 

 void set_DACZ(int16_t value); 

 void set_DACZero();
 
// virtual void SetPIDMode(uint8_t mode)=0;
 
 void reset_ADCPort();
 
 void move_scannerX(int x);

 void move_scannerY(int y);

 //uint16_t *getValuesFromAdc();  // чтение АЦП
 void      getValuesFromAdc();  // чтение АЦП

 uint32_t  ReadDataFromFPGA(FPGAReadData readdata);
 
 void ReadDataFromFPGAArray();
 
 void retract();       // втянуть сканер

 void retract(int16_t HeightJump); //втянуть на HeightJump

 void protract();      // вытянуть сканер
  
 void freezeLOOP(uint16_t delay);    // заморозить ПИД

 void unfreezeLOOP(uint16_t delay);  // разморозить ПИД 

 // void protract(uint16_t delay,int16_t DacZ0,int16_t HeightJump) ; //разморозить ПИД 
 
};
#endif
