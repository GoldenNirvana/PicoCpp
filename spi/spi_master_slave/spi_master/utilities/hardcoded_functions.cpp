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
  ADC_IS_READY_TO_READ = false;
  conv.disable();
  sleep_us(10);
  conv.enable();
}

void IniSPI( uint8_t port ,uint8_t v2 ,uint8_t v3, uint8_t v4 )
{
 decoder.activePort(port);
 Spi::setProperties(v2, v3, v4);
}
void Init_DAC1(uint8_t port)
{
  dac8563_1.initialize(port); //code 23
  afc.clear();
  afc = "debug Init DAC1 " + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
}

void Init_DAC2(uint8_t port)
{
  dac8563_2.initialize(port); //code 27
  afc.clear();
  afc = "debug Init DAC2 " + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
}

void set_Bias(int8_t chanel,int16_t Bias)
{
//   code  22 , 2, 8, 0, 1, 1, value 
    if (chanel == 0)
      {
        dac8563_1.writeA(Bias);
      }
      else 
      if (chanel == 1)
      {
        dac8563_1.writeB(Bias);
      }	
 /*  отладка
   afc.clear();
  afc = "debug Bias " + std::to_string(chanel) + ',' + std::to_string(Bias);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
  */
}

void set_SetPoint(int8_t chanel, int16_t SetPoint)
{//  code  22, 2, 8, 0, 1, 0, value
   if (chanel == 0)
   {
     dac8563_1.writeA(SetPoint);
   }
   else 
   if (chanel == 1)
   {
     dac8563_1.writeB(SetPoint);
    }	
  /* отладка
  afc.clear();
  afc = "debugSetPoint " + std::to_string(chanel) + ',' + std::to_string(SetPoint);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
  */
}
void set_gain(int gain, int p)
{
  uint8_t intBuf[1];
  decoder.activePort(p);
  Spi::setProperties(8, 0, 0);
  intBuf[0] = gain;
  spi_write_blocking(spi_default, intBuf, 3);
}
void set_gainPID(int gain)
{
   set_io_value(2, gain); 
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
  ADC_ENABLE_DISABLE = false;
  ADC_RESET = false;
  ADC_READ = false;
  ADC_READ_FOREVER = false;
  ADC_GET_VALUE = false;
  ADC_IS_READY_TO_READ = true;
  FREQ_SET = false;
  AD8400_SET_GAIN = false;
  AD8400_SENDER = false;
  AD5664 = false;
  SET_BIAS=false;
  SET_SETPOINT=false;
  SET_XY=false;
  InitDAC_BIAS_SET_POINT=false;
  InitDAC_XY=false;
  SCANNING = false;
  FASTSCANNING = false;
  APPROACH = false;
  CONFIG_UPDATE = false;
  MOVE_TOX0Y0 = false;
  STOP_ALL = true;
  SET_IO_VALUE = false;
  SCANNER_RETRACT_PROTRACT = false;
  LID = false;
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
  while (!ADC_IS_READY_TO_READ && i++ < 3)
  {
//    std::cout << "EndlessLoop\n";
    sleep_us(100);
  }
  return spiBuf;
}
/*
uint16_t *getValuesFromAdc()
{
  return repeatTwoTimes();
}
*/

uint16_t *repeatTwoTimes()
{
  get_result_from_adc();
  int i = 0;
  while (!ADC_IS_READY_TO_READ && i++ < 3)
  {
    sleep_us(100);
  }
  return spiBuf;
}

