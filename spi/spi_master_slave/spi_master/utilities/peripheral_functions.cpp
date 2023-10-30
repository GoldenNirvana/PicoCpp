#include "peripheral_functions.hpp"
#include "../loop/common_data/common_variables.hpp"
#include "../transceiver/rx_core.hpp"
#include "hardcoded_functions.hpp"
#include <pico/multicore.h>
#include <iostream>
#include <bitset>


#define UART_TX_PIN 8
#define UART_RX_PIN 9


void setDefaultSettings()
{
  /// BASIC SETTINGS

  uart_init(uart1, 115200);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  gpio_pull_down(resetPort.getPort());

#warning should be undeleted
//  RX_core rxCore;

  // fixme mb should add & before isr
  gpio_set_irq_enabled_with_callback(busy.getPort(), GPIO_IRQ_EDGE_FALL, true, RX_core::comReceiveISR);

  
  multicore_reset_core1();
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
}

void activateError()
{
  while (true)
  {
    ledPort.enable();
    activateRed();
    sleep_ms(1000);
    ledPort.disable();
    activateDark();
    sleep_ms(1000);
  }
}

void _delay_us(double __us)
{
  uint32_t __count =
    (uint32_t) (__us / 0.008) - 3; // 8ns per cycle for 125MHz, from experimentation remove 3cycles for overhead
  busy_wait_at_least_cycles(__count);
}

void green()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateGreen();
    sleep_ms(100);
  }
}

void blue()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateBlue();
    sleep_ms(100);
  }
}

void red()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateRed();
    sleep_ms(100);

  }
}

void dark()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
  }
}


void activateGreen()
{

  rdbLed.disable();
  sleep_us(60);
  for (int i = 0; i < 8; ++i)
  {
    rdbLed.enable();
    busy_wait_at_least_cycles(85);
    rdbLed.disable();
    busy_wait_at_least_cycles(35);
  }

  for (int i = 0; i < 16; ++i)
  {
    rdbLed.enable();
    busy_wait_at_least_cycles(35);
    rdbLed.disable();
    busy_wait_at_least_cycles(85);
  }
}

void activateRed()
{
  rdbLed.disable();
  sleep_us(60);
  for (int i = 0; i < 8; ++i)
  {
    rdbLed.enable();
    busy_wait_at_least_cycles(35);
    rdbLed.disable();
    busy_wait_at_least_cycles(85);
  }

  for (int i = 0; i < 8; ++i)
  {
    rdbLed.enable();
    busy_wait_at_least_cycles(85);
    rdbLed.disable();
    busy_wait_at_least_cycles(35);
  }

  for (int i = 0; i < 8; ++i)
  {
    rdbLed.enable();
    busy_wait_at_least_cycles(35);
    rdbLed.disable();
    busy_wait_at_least_cycles(85);
  }
}

void activateBlue()
{
  rdbLed.disable();
  sleep_us(60);
  for (int i = 0; i < 16; ++i)
  {
    rdbLed.enable();
    busy_wait_at_least_cycles(35);
    rdbLed.disable();
    busy_wait_at_least_cycles(85);
  }
  for (int i = 0; i < 8; ++i)
  {
    rdbLed.enable();
    busy_wait_at_least_cycles(85);
    rdbLed.disable();
    busy_wait_at_least_cycles(35);
  }
}

void activateDark()
{
  rdbLed.disable();
  sleep_us(60);
  for (int i = 0; i < 24; ++i)
  {
    rdbLed.enable();
    busy_wait_at_least_cycles(35);
    rdbLed.disable();
    busy_wait_at_least_cycles(85);
  }
}

void moveLinearDriverUntilStop(int lid_name, int f, int p, int n, int dir)
{
  uint32_t SET_POINT, GATE_Z_MAX, GATE_Z_MIN;
   int8_t status;
    //  GATE_Z_MAX = vector[2];
    //   GATE_Z_MIN = vector[3];
  const int  none = 30;
  const int    ok = 3;
  const int touch = 2;
  if (lid_name == 90 || lid_name == 95)
  {
    while (LID_UNTIL_STOP)
    {
      status=none;
      ad7606Value=32000;
      ad7606SignalValue=32000;
      linearDriver.activate(lid_name, f, p, n, dir);
      afc.clear(); //231025
      afc="code"+std::to_string(lid_name)+','+std::to_string(status)+','+std::to_string(ad7606Value)+','+std::to_string(ad7606SignalValue)+"\n";
    //  afc="code"+std::to_string(lid_name)+"\n";
      std::cout <<afc;
      afc.clear();
    }
      afc.clear();
      sleep_ms(200);
      std::cout <<"end\n";      
  }
  if (lid_name == 99)
  {
     status=none;
    while (LID_UNTIL_STOP)
    {
      Z_STATE = true;
      if (!flgVirtual) //231025
      {
        get_result_from_adc();
       while (Z_STATE)
       {
        sleep_us(1000);
       }
       if(ad7606Value<GATE_Z_MIN)
       {
         status=touch;
         break;
       }
       if(ad7606Value<GATE_Z_MAX)
       {
         status=ok;
         break;
       } 
           // check if z > <
       linearDriver.activate(lid_name, f, p, n, dir);
      }
      else
      {
        ad7606Value=20000;
        ad7606SignalValue=20000;
      }
      afc.clear();//231025
      afc="code"+std::to_string(lid_name)+','+std::to_string(status)+','+std::to_string(ad7606Value)+','+std::to_string(ad7606SignalValue)+"\n";
      std::cout <<afc;
      afc.clear();
    }
      afc.clear();//231025
      afc="code"+std::to_string(lid_name)+','+std::to_string(status)+','+std::to_string(ad7606Value)+','+std::to_string(ad7606SignalValue)+"\n";
      std::cout <<afc;
      afc.clear();
      sleep_ms(200);
      std::cout <<"end\n";      
  }
}
void set_io_value(int port, int value)
{
  SET_IO_VALUE = false;
  if (port == 1)
  {
    std::string binary = std::bitset<2>(value).to_string();
    binary[1] == '1' ? io1_0.enable() : io1_0.disable();
    binary[0] == '1' ? io1_1.enable() : io1_1.disable();
  } else if (port == 2)
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