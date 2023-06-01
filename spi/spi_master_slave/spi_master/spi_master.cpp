#include <cstdio>
#include <pico/multicore.h>
#include "pico/stdlib.h"
#include "Parser.h"
#include "Spi.hpp"
#include "Decoder.hpp"
#include "Utilities.hpp"
#include "hardware/irq.h"
#include "hardware/gpio.h"

bool AD9833_SENDER = false;
bool AD8400_SENDER = false;
bool AD7606_ENABLE_DISABLE = false;
bool AD7606_RESET = false;
bool AD7606_READ = false;
bool AD5664_SENDER = false;

bool AD7606_READ_FOREVER = false;

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
      default:
        error
    }
  }
}

int main()
{
  int port = 9;
  gpio_init(port);
  gpio_set_dir(port, GPIO_IN);
  gpio_pull_down(resetPort.getPort());
  gpio_set_irq_enabled_with_callback(port, GPIO_IRQ_EDGE_FALL, true, comReceiveISR);

  multicore_launch_core1(launchOnCore1);

  /// BASIC SETTINGS
  dec.enable();
  conv.enable();
  resetPort.disable();
  gpio_pull_down(resetPort.getPort());
  ledPort.enable();
  /// MAIN LOOP
  while (true)
  {
    decoder.activePort(vector[1]);
    Spi::setProperties(vector[2], vector[3], vector[4]);
    /// MAIN IF
    if (AD9833_SENDER)
    {
      AD9833_SENDER = false;
      uint8_t buf[6];
      for (int j = 0; j < 6; ++j)
      {
        buf[j] = vector[5 + j];
      }
      Spi::write(buf, 6);
    }
    if (AD8400_SENDER)
    {
      AD8400_SENDER = false;
      uint8_t inBuf[1];
      inBuf[0] = vector[5];
      spi_write_blocking(spi_default, inBuf, 1);
    }
    if (AD7606_ENABLE_DISABLE)
    {
      AD7606_ENABLE_DISABLE = false;
      if (vector[5] == 1)
      {
        AD7606_READ_FOREVER = false;
      }
      else if (vector[5] == 0)
      {
        AD7606_READ_FOREVER = true;
      }
    }
    if (AD7606_RESET)
    {
      AD7606_RESET = false;
      resetPort.enable();
      sleep_us(10);
      resetPort.disable();
    }
    if (AD7606_READ or AD7606_READ_FOREVER)
    {
      AD7606_READ = false;
      conv.enable();
      conv.disable();
      sleep_us(10);
      conv.enable();
    }
    if (AD5664_SENDER)
    {
      AD5664_SENDER = false;
      uint8_t inBuf1[3];
      inBuf1[0] = vector[5];
      inBuf1[1] = vector[6];
      inBuf1[2] = vector[7];
      dec.disable();
      spi_write_blocking(spi_default, inBuf1, 3);
      dec.enable();
    }
  }
}
