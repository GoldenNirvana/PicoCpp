#ifndef PICO_EXAMPLES_UTILITIES_HPP
#define PICO_EXAMPLES_UTILITIES_HPP


#include <iostream>
#include <iomanip>
#include <pico/bootrom.h>

Spi spi;
Decoder decoder(4, 5, 6);

static uint16_t inputBuf[8];
static int vector[10];

bool AD9833_SENDER = false;
bool AD8400_SENDER = false;
bool AD7606_ENABLE_DISABLE = false;
bool AD7606_RESET = false;
bool AD7606_READ = false;
bool AD5664_SENDER = false;
bool AD7606_READ_FOREVER = false;
bool AD7606_IS_SCANNING = false;

GpioPort conv(7);
GpioPort dec(10);
GpioPort resetPort(8);
GpioPort ledPort(PICO_DEFAULT_LED_PIN);

[[noreturn]] void activateError();
void serialPrintBuffer(const uint16_t *const buf, int len);
void parse(int *vector);
void comReceiveISR(uint a, uint32_t b);
void launchOnCore1();

void comReceiveISR(uint a, uint32_t b)
{
  spi_read16_blocking(spi_default, 0, inputBuf, 8);
  serialPrintBuffer(inputBuf, 8);
}

void launchOnCore1()
{
  while (true)
  {
    /// PARSING
    parse(vector);
    switch (vector[0])
    {
      case 1:
        AD9833_SENDER = true;
        break;
      case 5:
        AD8400_SENDER = true;
        break;
      case 6:
        AD7606_ENABLE_DISABLE = true;
        break;
      case 11:
        AD7606_RESET = true;
        break;
      case 12:
        AD7606_READ = true;
        break;
      case 20:
        AD5664_SENDER = true;
        break;
      case 25:
        AD7606_IS_SCANNING = true;
        break;
      default:
        activateError();
    }
  }
}

void serialPrintBuffer(const uint16_t *const buf, int len)
{
  uint32_t x = time_us_64();
//  std::cout << '[' << (int) (x / 1000000) << '.' << int(x % 1000000) << "] ";
  std::cout << '[' << std::fixed << std::setfill('0') << std::setw(10) << std::right << x << "]     ";
  std::cout << std::resetiosflags(std::ios_base::right);
  std::cout << std::resetiosflags(std::ios_base::fixed);
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

void activateError()
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
