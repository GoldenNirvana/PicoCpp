#include <cstdio>
#include <pico/multicore.h>
#include "pico/stdlib.h"
#include "Parser.h"
#include "Spi.hpp"
#include "Decoder.hpp"
#include "Utilities.hpp"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "peripheral_functions.hpp"


int main()
{
  int port = 9; // Busy port
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
    if (X_LID)
    {
      X_LID = false;
      static uint16_t inBuf[4]; // f, p, n, d
      for (int j = 0; j < 4; ++j)
      {
        inBuf[j] = vector[1 + j];
      }
      xLID.activate(inBuf[0], inBuf[1], inBuf[2], inBuf[3]);
      continue;
    }
    if (AD7606_IS_SCANNING)
    {
      static uint16_t inBuf[4]; // n, start_freq, step, channel
      if (!is_already_scanning)
      {
        for (int j = 0; j < 4; ++j)
        {
          inBuf[j] = vector[1 + j];
        }
//        serialPrintBuffer(inBuf, 4);
        current_channel = inBuf[3] - 1;
        is_already_scanning = true;
      }
      else
      {
        if (scan_index++ < inBuf[0])
        {
          set_freq(inBuf[1]);
          get_result_from_adc();
          sleep_ms(10); // TODO
          inBuf[1] += inBuf[2];
        }
        else
        {
          std::cout << afc << '\n';
          scan_index = 0;
          current_channel = 0;
          current_freq = 0;
          AD7606_IS_SCANNING = false;
          is_already_scanning = false;
        }
      }
      continue;
    }
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
      spi_write_blocking(spi_default, buf, 6);
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
