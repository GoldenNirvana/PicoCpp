#include "loop/common_data/common_variables.hpp"
#include "loop/main_core.hpp"
#include "utilities/peripheral_functions.hpp"
#include "devices/DAC8563.hpp"
#include <cstring>
#include <iostream>

int start_app()
{
  critical_section_init(&criticalSection);
  if (!critical_section_is_initialized(&criticalSection))
  {
    activateError();
  }
  setDefaultSettings();
  MainCore mainCore;
  mainCore.loop();
  return 0;
}

int main()
{

//  for (int i = 0; ; ++i)
//  {
//    port.enable();
//    busy_wait_at_least_cycles(1);
//    busy_wait_at_least_cycles(1);
//    busy_wait_at_least_cycles(1);
////    _delay_us(100);
//    port.disable();
//    busy_wait_at_least_cycles(1);
//    busy_wait_at_least_cycles(1);
//    busy_wait_at_least_cycles(1);
////    _delay_us(100);
//  }

//  Spi::setProperties(8, 0, 1, SPI_MSB_FIRST);
//  decoder.activePort(7); // ???
//  uint16_t value = 1000;
//  DAC8563 dac8563;
//
//  int i = 1;
//  while (i++ < 100000)
//  {
//    std::cout << value << '\n';
//    dac8563.writeVoltage(value);
//    value += 100;
//    if (value > 65000)
//    {
//      value = 1000;
//    }
//    sleep_ms(10);
//  }
//  std::cout << "EXIT \n";


  return start_app();
}
