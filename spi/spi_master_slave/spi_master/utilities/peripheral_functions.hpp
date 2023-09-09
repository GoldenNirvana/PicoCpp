#ifndef PICO_EXAMPLES_peripheral_FUNCTIONS_HPP
#define PICO_EXAMPLES_peripheral_FUNCTIONS_HPP

#include <vector>
#include "../Spi.hpp"
#include "../devices/ad5664.hpp"
#include "../Utilities.hpp"


Spi spi;
Decoder decoder(4, 5, 6);

void set_freq(uint16_t freq);
void get_result_from_adc();
void set_gain(int gain, int p = 2);
void set_clock_enable();
void set_on_cap(uint8_t channel, uint16_t value);


#endif
