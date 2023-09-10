#ifndef PICO_EXAMPLES_RX_CORE_HPP
#define PICO_EXAMPLES_RX_CORE_HPP

#include <ctime>

class RX_core
{
public:
  static void serialPrintBuffer(const uint16_t *buf, int len);
  static void serialPrintBuffer(const uint8_t *buf, int len);

  static void parse(int32_t *vector);

  static void comReceiveISR(uint a, uint32_t b);

  static void launchOnCore1();

private:


};


#endif
