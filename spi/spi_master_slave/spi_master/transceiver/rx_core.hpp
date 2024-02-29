#ifndef PICO_EXAMPLES_RX_CORE_HPP
#define PICO_EXAMPLES_RX_CORE_HPP

#include <ctime>
#include <vector>

class RX_core
{
 public:
  static void serialPrintBuffer(const uint16_t *buf, int len);
  static void serialPrintBuffer(const uint8_t *buf, int len);
  static void serialPrint2Buffer(const uint16_t *const buf);
 // static void parse(int32_t *vector);
  static void parse(std::vector<int32_t> &vec);
  static void parse(std::vector<int32_t> &vec,std::vector<int32_t> &vparams);
  static void comReceiveISR(uint a, uint32_t b);
  static void launchOnCore1();
private:
 };


#endif
