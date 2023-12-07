#include "hardcoded_functions.hpp"
#include "../loop/common_data/common_variables.hpp"
#include "../transceiver/rx_core.hpp"
#include <pico/multicore.h>
#include <iostream>
#include "../utilities/debug_logger.hpp"
#include "peripheral_functions.hpp"


void set_freq(uint32_t freq)
{
  int64_t flag_freq = 1 << 14;
  int64_t scale = 1 << 28;
  int64_t n_reg = int64_t(freq * scale / 25.0e6);
  int64_t n_low = n_reg & 0x3fff;
  int64_t n_hi = (n_reg >> 14) & 0x3fff;

  uint8_t buf[6];

  buf[0] = (flag_freq | n_low) / (0x100);
  buf[1] = (flag_freq | n_low) % (0x100);
  buf[2] = (flag_freq | n_hi) / (0x100);
  buf[3] = (flag_freq | n_hi) % (0x100);
  buf[4] = (0x2000) / (0x100);
  buf[5] = (0x2000) % (0x100);

  logger(buf, 6);

  decoder.activePort(1);
  Spi::setProperties(8, 1, 1);
  spi_write_blocking(spi_default, buf, 2);
  spi_write_blocking(spi_default, buf + 2, 2);
  spi_write_blocking(spi_default, buf + 4, 2);

}

void get_result_from_adc()
{
  AD_7606_IS_READY_TO_READ = false;
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

void set_on_dac(uint8_t channel, uint16_t value)
{
  dac8563_1.setSpiProps();
//  AD56X4Class::setChannel(AD56X4_SETMODE_INPUT, channel, value);
//  AD56X4Class::updateChannel(channel);
  channel--;
  if (channel == 0)  dac8563_1.writeA(value);
  if (channel == 1)  dac8563_1.writeB(value);
}

void stopAll()
{
  AD9833_SENDER = false;
  AD8400_SENDER = false;
  AD7606_ENABLE_DISABLE = false;
  AD7606_RESET = false;
  AD7606_READ = false;
  AD7606_READ_FOREVER = false;
  AD7606_GET_VALUE = false;
  AD_7606_IS_READY_TO_READ = true;
  FREQ_SET = false;
  AD8400_SET_GAIN = false;
  AD8400_SENDER = false;
  AD5664 = false;
  DAC8563_SET_VOLTAGE_1 = false;
  DAC8563_INIT_1 = false;
  DAC8563_INIT_2 = false;
  SCANNING = false;
  FASTSCANNING = false;
  APPROACH = false;
  CONFIG_UPDATE = false;
  MOVE_TOX0Y0 = false;
  STOP_ALL = true;
  SET_IO_VALUE = false;
  SET_ONE_IO_VALUE = false;
  LID = false;
  DAC8563_SET_VOLTAGE_1 = false;
  LID_UNTIL_STOP = false;
  RESONANCE = false;
  RESONANCE_STOP = true;
  RESONANCE_ACTIVE = false;
  afc.clear();
}

uint16_t *getValuesFromAdc()
{
  get_result_from_adc();
  int i = 0;
  while (!AD_7606_IS_READY_TO_READ && i++ < 3)
  {
    sleep_us(100);
  }
  return spiBuf;
}

