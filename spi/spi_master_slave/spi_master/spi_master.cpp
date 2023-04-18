#include <cstdio>
#include "pico/stdlib.h"
#include "Parser.h"
#include "Spi.hpp"
#include "Decoder.hpp"
#include "pico/multicore.h"
#include "Utilities.hpp"

int main()
{
  Decoder decoder(4, 5, 6);
  GpioPort conv(7);
  GpioPort dec(10);
  dec.enable();
  conv.enable();
  Spi spi;
  GpioPort resetPort(9);
  resetPort.disable();
  GpioPort ledPort(PICO_DEFAULT_LED_PIN);
  ledPort.enable();
  static int vector[10];


//  multicore_launch_core1(core1);

  /// MAIN LOOP
  while (true)
  {
    /// PARSING
    parse(vector);
    /// SET PROPERTIES
    decoder.activePort(vector[1]);
    Spi::setProperties(vector[2], vector[3], vector[4]);
    /// MAIN SWITCH
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
      case 5: // ad8400
        uint16_t inBuf[1];
        inBuf[0] = vector[5];
        spi_write16_blocking(spi_default, inBuf, 1);
        break;
      case 6:
        if (vector[5] == 1)
          ADC = false;
        if (vector[5] == 0)
        {
          ADC = true;
        }
        break;
      case 11:
        resetPort.enable();
        sleep_us(1);
        resetPort.disable();
        break;
      case 12:
        conv.enable();
        conv.disable();
        sleep_us(10);
        conv.enable();
        sleep_us(500); // todo
        spi_read16_blocking(spi_default, 0, inputBuf, 8);
        serialPrintBuffer(inputBuf, 8);
        sleep_ms(1);
        break;
      case 20:
        uint8_t inBuf1[3];
        inBuf1[0] = vector[5];
        inBuf1[1] = vector[6];
        inBuf1[2] = vector[7];
        dec.disable();
        spi_write_blocking(spi_default, inBuf1, 3);
        dec.enable();
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
