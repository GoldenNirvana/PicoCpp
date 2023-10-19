#include "hardcoded_functions.hpp"
#include "../loop/common_data/common_variables.hpp"
#include "../transceiver/rx_core.hpp"
#include <pico/multicore.h>
#include <iostream>

void set_freq(uint32_t freq)
{
  current_freq = freq;

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

//  serialPrintBuffer(buf, 6);

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

void set_on_cap(uint8_t channel, uint16_t value)
{
  decoder.activePort(3);
  dac8563.setSpiProps();
//  AD56X4Class::setChannel(AD56X4_SETMODE_INPUT, channel, value);
//  AD56X4Class::updateChannel(channel);
  channel--;
  if (channel == 0)
    dac8563.writeA(value);
  if (channel == 1)
    dac8563.writeB(value);
}

void stopAll()
{
  AD9833_SENDER = false;
  AD8400_SENDER = false;
  AD7606_ENABLE_DISABLE = false;
  AD7606_RESET = false;
  AD7606_READ = false;
  AD7606_READ_FOREVER = false;
  AD7606_STOP_SCAN = true;
  AD9833_SET_FREQ = false;
  AD8400_SET_GAIN = false;
  AD7606_GET_VALUE = false;
  AD5664 = false;
  MICRO_SCAN = false;
  CONFIG_UPDATE = false;
  MOVE_TO = false;
  STOP_ALL = true;
  SET_IO_VALUE = false;
  SET_ONE_IO_VALUE = false;
  LID = false;
  AD7606_TRIG_GET_VALUE = false;
  AD7606_GET_ALL_VALUES = false;
  DAC8563_SET_VOLTAGE = false;
  DAC8563_INIT = false;
  LID_UNTIL_STOP = false;

  AD_7606_IS_READY_TO_READ = true;
  AD7606_IS_SCANNING = false;
  is_already_scanning = false;
  scan_index = 0;
  current_freq = 0;
  current_channel = 0;
  afc.clear();
}

uint16_t *getValuesFromAdc()
{
  get_result_from_adc();
  while (!AD_7606_IS_READY_TO_READ)
  {
    sleep_us(1000);
  }
  return spiBuf;
}

void approacphm(const uint32_t *const data)
{
  int M_A;
  int M_BASE;
  int M_smZ_ctrl;
  int M_smZ_status;
  int M_PID_out;
  int M_I;
  int M_sm_speed;
  int M_GAIN;
  const int steps = 50;
  const int done = 60;
  const int waitsteps = 40;
  const int none = 30;
  const int stop = 100;
  const int MakeSTOP = 1;
  const int stopdone = 5;

  uint32_t SMZ_STEP, SIGNAL, Z;
  uint32_t SET_POINT, GATE_Z_MAX, GATE_Z_MIN;
  int32_t GAIN, NSTEPS, STMFLG, SPEED;
  uint32_t INTDELAY, SCANNERDECAY;

  uint32_t lstatus;
  // SET VALUE FROM RX_CORE
  SET_POINT = data[0];
  GATE_Z_MAX = data[1];
  GATE_Z_MIN = data[2];
  NSTEPS = data[3];
  INTDELAY = data[4];
  GAIN = data[5];
  SPEED = data[6];
  SCANNERDECAY = data[7];
  STMFLG = data[8];
  dac8563.writeA(SET_POINT);
  uint32_t buf_step = waitsteps;
  std::vector<uint32_t> buf_params;
  buf_params.reserve(7);
  for (int i = 0; i < 7; ++i)
    buf_params.push_back(data[i]);

  auto ptr = getValuesFromAdc();
  SIGNAL = ptr[0];
  Z = ptr[1];

  std::vector<uint32_t> buf_status;
  buf_status.push_back(none);
  buf_status.push_back(Z);
  buf_status.push_back(SIGNAL);

  uint32_t buf_stop = waitsteps;

  while (true)
  {
    buf_status[0] = none;
    if (STOP_ALL)
    {
      buf_status[0] = stopdone;
      buf_status[1] = Z;
      buf_status[2] = SIGNAL;
      io3_1.disable();
      break;
    }
    if (CONVERGENCE_CONFIG_UPDATE)
    {
      SET_POINT = vector[1];
      GATE_Z_MAX = vector[2];
      GATE_Z_MIN = vector[3];
      NSTEPS = vector[4];
      INTDELAY = vector[5];
      GAIN = vector[6];
      SPEED = vector[7];
    }

    dac8563.writeA(SET_POINT);
    set_gain(GAIN);

    sleep_ms(INTDELAY);

    // TODO ??? find bugs
    if (NSTEPS >= 0)
    {
      ptr = getValuesFromAdc();
      Z = ptr[0];
      SIGNAL = ptr[1];
      if (Z <= GATE_Z_MIN)
      {
        buf_status[0] = 2; // touch
        buf_status[1] = Z;
        buf_status[2] = SIGNAL;
        break;
      }
      if (Z <= GATE_Z_MAX)
      {
        int k = 0;
        for (int i = 0; i < 10; ++i)
        {
          Z = getValuesFromAdc()[0];
          if (Z <= GATE_Z_MAX)
            k++;
          sleep_ms(10);
        }
        if (k >= 3)
        {
          buf_status[0] = 3; // ok
          buf_status[1] = Z;
          buf_status[2] = SIGNAL;
        }
        break;
      }
    }
    if (NSTEPS < 0)
    {
      io3_1.disable();
      sleep_ms(SCANNERDECAY);
    }

    std::cout << "code75," << buf_status[0] << ',' << buf_status[1] << ',' << buf_status[2] << '\n';


    linearDriver.activate(99, 500, 500, std::abs(NSTEPS), NSTEPS > 0);
  }

  std::cout << "code75," << buf_status[0] << ',' << buf_status[1] << ',' << buf_status[2] << '\n';
  sleep_ms(100);
  std::cout << "end\n";
}
