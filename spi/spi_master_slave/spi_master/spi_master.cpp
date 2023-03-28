#include <cstdio>
#include "pico/stdlib.h"
#include "Parser.h"
#include "Spi.hpp"
#include "Decoder.hpp"

#define DEBUG  0

void serialPrintBuffer(const uint16_t *const buf, int len)
{
  for (int i = 0; i < len; ++i)
  {
    printf("%u ", buf[i]);
  }
}

int main()
{
  Decoder decoder(4, 5, 6);
  Spi spi;
  GpioPort resetPort(9);
  resetPort.disable();
  GpioPort conv(7);
  conv.enable();
//  InputPort buzy(6);

  // INDICATOR
  GpioPort ledPort(PICO_DEFAULT_LED_PIN);
  ledPort.enable();

  char msg[BUF_LEN];

  // MAIN LOOP
  while (true)
  {
    scanf("%20s", msg);
    // PARSING
    Parser parser(msg, ',');
    static int vector[10];
    static uint16_t inputBuf[8];
    for (int i = 0; i < 8; ++i)
    {
      inputBuf[i] = 0;
    }

    for (int c = 0; c < 10; c++)
    {
      vector[c] = -1;
    }
    parser.parseInts(vector);
    if (DEBUG != 0)
    {
      for (int c = 0; c < 10; c++)
      {
        printf("%i", vector[c]);
        printf(" ");
      }
    }
    // SET PROPERTIES
    int c = 0;

    decoder.activePort(vector[1]);
    Spi::setProperties(vector[2], vector[3], vector[4]);
    // MAIN SWITCH
    switch (vector[0])
    {
      case 0:
        break;
      case 1: // AD9833
        uint8_t buf[2];
        for (int j = 0; j < 2; ++j)
        {
          buf[j] = vector[5 + j];
        }
        Spi::write(buf, 2);
        break;
      case 5:
        uint16_t inBuf[1];
        inBuf[0] = vector[5];
        spi_write16_blocking(spi_default, inBuf, 1);
      case 11:
        resetPort.enable();
        sleep_us(1);
        resetPort.disable();
        break;
      case 12:
        conv.disable();
        sleep_us(1);
        conv.enable();
        sleep_us(500); // todo
        spi_read16_blocking(spi_default, 0, inputBuf, 8);
        serialPrintBuffer(inputBuf, 8);
        break;
      default:
        while (true)
        {
          ledPort.enable();
          sleep_ms(100);
          ledPort.disable();
          sleep_ms(100);
        }
    }
  }
}
