#ifndef PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP
#define PICO_EXAMPLES_HARDCODED_FUNCTIONS_HPP

#include "../devices/ad5664.hpp"

// WARNING HARDCODED FUNCTIONS

void set_freq(uint32_t freq);

uint16_t *getValuesFromAdc();

void get_result_from_adc();

void set_gain(int gain, int p = 2);

void set_clock_enable();

void set_on_dac(uint8_t channel, uint16_t value);

void stopAll();

void approacphm(const uint32_t *data);

#endif
