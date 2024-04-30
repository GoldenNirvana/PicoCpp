#ifndef PICO_EXAMPLES_LED_CONTROLLER_HPP
#define PICO_EXAMPLES_LED_CONTROLLER_HPP

#include <cstdint>
#include <hardware/clocks.h>

void activateError();

void activateGreen();
void activateRed();
void activateBlue();
void activateDark();

void green();
void blue();
void red();
void dark();

#endif
