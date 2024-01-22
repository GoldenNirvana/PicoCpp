#include "hardcoded_functions.hpp"
#include <pico/multicore.h>
#include <iostream>
#include "../loop/common_data/common_variables.hpp"
#include "../transceiver/rx_core.hpp"
#include "../utilities/debug_logger.hpp"
#include "peripheral_functions.hpp"

#include <pico/multicore.h>
#include <bitset>

#define UART_TX_PIN 8
#define UART_RX_PIN 9

void set_io_value(int port, int value)  
{
  SET_IO_VALUE = false;
  if (port == 1)
  {
    std::string binary = std::bitset<2>(value).to_string();
    binary[1] == '1' ? io1_0.enable() : io1_0.disable();
    binary[0] == '1' ? io1_1.enable() : io1_1.disable();
  } else if (port == 2) //gain
  {
    std::string binary = std::bitset<3>(value).to_string();
    binary[2] == '1' ? io2_0.enable() : io2_0.disable();
    binary[1] == '1' ? io2_1.enable() : io2_1.disable();
    binary[0] == '1' ? io2_2.enable() : io2_2.disable();
  } else if (port == 3)
  {
    std::string binary = std::bitset<2>(value).to_string();
    binary[1] == '1' ? io3_0.enable() : io3_0.disable();
    binary[0] == '1' ? io3_1.enable() : io3_1.disable();
  }
}
void setDefaultSettings()
{
  /// BASIC SETTINGS

  uart_init(uart1, 115200);
 // uart_init(uart0, 256000);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  gpio_pull_down(resetPort.getPort());

//#warning should be undeleted
//  RX_core rxCore;

  // fixme mb should add & before isr
  gpio_set_irq_enabled_with_callback(busy.getPort(), GPIO_IRQ_EDGE_FALL, true, RX_core::comReceiveISR);

  multicore_launch_core1(RX_core::launchOnCore1);

  dec.enable();
  conv.enable();
  resetPort.disable();
  gpio_pull_down(resetPort.getPort());
  ledPort.enable();
  io3_1.disable();

  // init io_ports, mb  delete
  io_ports.push_back(io1_0);
  io_ports.push_back(io1_1);
  io_ports.push_back(io2_0);
  io_ports.push_back(io2_1);
  io_ports.push_back(io2_2);
  io_ports.push_back(io3_0);
  io_ports.push_back(io3_1);
  //io_ports.push_back(io3_2);
}
void set_Freq(uint32_t freq)
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
void init_DACSPB(uint8_t port) //  4 для подставки
{
  dac8563_1.initialize(port); //code 23
  afc.clear();
  afc = "debug Init DACSPB " + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
}

void init_DACXY(uint8_t port)
{
  dac8563_2.initialize(port); //code 27
  afc.clear();
  afc = "debug Init DACXY " + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
}

void init_DACZ(uint8_t port)
{
  dac8563_2.initialize(port); //code 27
  afc.clear();
  afc = "debug Init DACZ " + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
}
void move_scannerX(int x)
{
 dac8563_2.writeA(x);
}
void move_scannerY(int y)
{
 dac8563_2.writeB(y);

}
void set_Bias(int8_t channel,int32_t Bias)
{
//   code  22 , 2, 8, 0, 1, 1, value 
    if (channel == 0)
      {
        dac8563_1.writeA(Bias);
      }
      else 
      if (channel == 1)
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

void set_SetPoint(int8_t channel, int32_t SetPoint)
{//  code  22, 2, 8, 0, 1, 0, value
   if (channel == 0)
   {
     dac8563_1.writeA(SetPoint);
   }
   else 
   if (channel == 1)
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
void set_GainApmlMod(int port, int gain)
{
  uint8_t intBuf[1];
  decoder.activePort(port);
  Spi::setProperties(8, 0, 0);
  intBuf[0] = gain;
  spi_write_blocking(spi_default, intBuf, 3);
}
void set_GainPID(int gain)
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

void set_DACXY(uint8_t channel, uint16_t value) 
{
  dac8563_2.setSpiProps();
//  AD56X4Class::setChannel(AD56X4_SETMODE_INPUT, channel, value);
//  AD56X4Class::updateChannel(channel);
  channel--;
  if (channel == 0)  dac8563_2.writeA(value);
  if (channel == 1)  dac8563_2.writeB(value);
}
void set_DACZ(uint16_t value) 
{


}
void stopAll()
{
  STOP=false;
}
/*
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
*/

uint16_t *getValuesFromAdc()
{
  repeatTwoTimes();
  return repeatTwoTimes();
}

uint16_t *repeatTwoTimes()
{
  get_result_from_adc();
  int j = 0;
  while (!ADC_IS_READY_TO_READ && j++ < 3)
  {
    sleep_us(100);
  }
  return spiBuf;
}

