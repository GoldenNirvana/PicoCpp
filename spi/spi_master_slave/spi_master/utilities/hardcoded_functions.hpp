#ifndef PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP
#define PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP

#include "../devices/ad5664.hpp"

// WARNING HARDCODED FUNCTIONS

void set_freq(uint32_t freq);

void IniSPI(uint8_t port ,uint8_t v2 ,uint8_t v3, uint8_t v4);

void Init_DAC1(uint8_t port);

void Init_DAC2(uint8_t port);

void set_Bias(int16_t Bias);

void set_SetPoint(uint16_t SetPoint);

uint16_t *repeatTwoTimes();

uint16_t *getValuesFromAdc();

void get_result_from_adc();

void set_gain(int gain, int p = 2); // set gain 

void set_gainPID(int gain);  //set gain PID

void set_clock_enable();

void set_on_dac(uint8_t channel, uint16_t value);

void stopAll();


#endif
