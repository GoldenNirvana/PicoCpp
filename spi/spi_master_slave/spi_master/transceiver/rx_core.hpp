#ifndef PICO_EXAMPLES_RX_CORE_HPP
#define PICO_EXAMPLES_RX_CORE_HPP

#include <ctime>

[[noreturn]] void activateError();

void serialPrintBuffer(const uint16_t *buf, int len);

void parse(int32_t *vector);

void comReceiveISR(uint a, uint32_t b);

void launchOnCore1();

void resetAll();

void setDefaultSettings();

#endif
