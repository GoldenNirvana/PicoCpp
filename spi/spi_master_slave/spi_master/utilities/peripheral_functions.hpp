#ifndef PICO_EXAMPLES_peripheral_FUNCTIONS_HPP
#define PICO_EXAMPLES_peripheral_FUNCTIONS_HPP

#include <cstdint>
#include <hardware/clocks.h>

// todo add command for reset all
void setDefaultSettings();

void moveLinearDriverUntilStop(int,int,int,int,int );
void set_io_value(int, int);

[[noreturn]] void activateError();

void activateGreen();
void activateRed();
void activateBlue();
void activateDark();

void green();
void blue();
void red();
void dark();


#endif
