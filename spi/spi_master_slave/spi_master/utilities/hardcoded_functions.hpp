#ifndef PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP
#define PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP

#include "../devices/ad5664.hpp"

// WARNING HARDCODED FUNCTIONS
void setDefaultSettings();

void set_io_value(int, int);

void set_freq(uint32_t freq); //установка заданной частоты генератора

void IniSPI(uint8_t port ,uint8_t v2 ,uint8_t v3, uint8_t v4); //инициирование SPI

void Init_DAC1(uint8_t port); //инициирование ЦАП1  SetPoint,BIAS

void Init_DAC2(uint8_t port); //инициирование ЦАП2  XY

void set_Bias(int8_t chanel,int32_t Bias); //установка заданного значения напряжения

void set_SetPoint(int8_t chanel,int32_t SetPoint);//установка заданной опроры для ПИД

void get_result_from_adc();       // чтение АЦП

void set_gain(int gain, int p = 2); // установить усиления 

void set_gainPID(int gain);          //установить усиления ПИД

void set_clock_enable();

void set_DACXY(uint8_t channel, uint16_t value); 

void stopAll(); //остановка  выполнения алгоритма

uint16_t *repeatTwoTimes();

uint16_t *getValuesFromAdc();  // чтение АЦП

#endif
