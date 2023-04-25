#ifndef PICO_EXAMPLES_UTILITIES_HPP
#define PICO_EXAMPLES_UTILITIES_HPP

#define error activateError();

#include <iostream>

static uint16_t inputBuf[8];
bool isADC = true;
GpioPort conv(7);
Spi spi;
Decoder decoder(4, 5, 6);
GpioPort dec(10);
GpioPort resetPort(9);
GpioPort ledPort(PICO_DEFAULT_LED_PIN);
static int vector[10];
[[noreturn]] void activateError();

void serialPrintBuffer(const uint16_t *const buf, int len)
{
  uint32_t x = time_us_64();
  std::cout << '[' << (int) (x / 1000000) << '.' << int(x % 1000000) / 10000 << "] ";
  for (int i = 0; i < len; ++i)
  {
    std::cout << buf[i] << ' ';
  }
  std::cout << "\n";
}

void parse(int *vector)
{
  std::string s;
  getline(std::cin, s);
  Parser parser(s.data(), ',');
  for (int c = 0; c < 10; c++)
  {
    vector[c] = -1;
  }
  parser.parseInts(vector);
  for (int i = 0; i < 8; ++i)
  {
    inputBuf[i] = 0;
  }
}

[[noreturn]] void activateError()
{
  while (true)
  {
    ledPort.enable();
    sleep_ms(100);
    ledPort.disable();
    sleep_ms(100);
  }
}

#endif
