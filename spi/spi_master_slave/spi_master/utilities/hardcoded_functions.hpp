#ifndef PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP
#define PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP

#include "../devices/ad5664.hpp"

// WARNING HARDCODED FUNCTIONS
void setDefaultSettings();

void set_io_value(int, int);

void set_freq(uint32_t freq); //установка заданной частоты генератора

void IniSPI(uint8_t port ,uint8_t v2 ,uint8_t v3, uint8_t v4); //инициирование SPI

void init_DACSPB(uint8_t port); //инициирование ЦАП1  SetPoint,BIAS

void init_DACXY(uint8_t port); //инициирование ЦАП2  XY

void init_DACZ(uint8_t port); //инициирование ЦАП1  DACZ

void set_Bias(int8_t chanel,int32_t Bias); //установка заданного значения напряжения

void set_SetPoint(int8_t chanel,int32_t SetPoint);//установка заданной опроры для ПИД

void get_result_from_adc();       // чтение АЦП

void set_Gain(int gain, int p = 2); // установить усиления 

void set_GainPID(int gain);          //установить усиления ПИД

void set_clock_enable();

void set_DACXY(uint8_t channel, uint16_t value); 

void set_DACZ(uint8_t channel,uint16_t value); 

void stopAll(); //остановка  выполнения алгоритма

void move_scannerX(int x);

void move_scannerY(int y);

uint16_t *repeatTwoTimes();

uint16_t *getValuesFromAdc();  // чтение АЦП

#endif
