#include "hardcoded_functions.hpp"
#include "../loop/common_data/common_variables.hpp"
#include "../transceiver/rx_core.hpp"
#include <pico/multicore.h>

void set_freq(uint32_t freq)
{
  current_freq = freq;

  int64_t flag_freq = 1 << 14;
  int64_t scale = 1 << 28;
  int64_t n_reg = int64_t(freq * scale / 25.0e6);
  int64_t n_low = n_reg & 0x3fff;
  int64_t n_hi = (n_reg >> 14) & 0x3fff;

  static uint8_t buf[6];

  buf[0] = (flag_freq | n_low) / (0x100);
  buf[1] = (flag_freq | n_low) % (0x100);
  buf[2] = (flag_freq | n_hi) / (0x100);
  buf[3] = (flag_freq | n_hi) % (0x100);
  buf[4] = (0x2000) / (0x100);
  buf[5] = (0x2000) % (0x100);

//  serialPrintBuffer(buf, 6);

  decoder.activePort(1);
  Spi::setProperties(8, 1, 1);
  spi_write_blocking(spi_default, buf, 2);
  spi_write_blocking(spi_default, buf + 2, 2);
  spi_write_blocking(spi_default, buf + 4, 2);

}

void get_result_from_adc()
{
  decoder.activePort(0);
  Spi::setProperties(16, 1, 0);
  conv.enable();
  conv.disable();
  sleep_us(10);
  conv.enable();
}

void set_gain(int gain, int p)
{
  uint8_t intBuf[1];
  decoder.activePort(p);
  Spi::setProperties(8, 0, 0);
  intBuf[0] = gain;
  spi_write_blocking(spi_default, intBuf, 3);
}

void set_clock_enable()
{
  uint8_t intBuf[1];
  Spi::setProperties(8, 1, 1);
  decoder.activePort(7);
  spi_write_blocking(spi_default, intBuf, 1);
}

void set_on_cap(uint8_t channel, uint16_t value)
{
  decoder.activePort(3);
  Spi::setProperties(8, 0, 1);
  AD56X4Class::setChannel(AD56X4_SETMODE_INPUT, channel, value);
  AD56X4Class::updateChannel(channel);
}