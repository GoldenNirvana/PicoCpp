#include <iostream>
#include "loop/common_data/common_variables.hpp"
#include "loop/main_core.hpp"
#include "utilities/peripheral_functions.hpp"
#include "hardware/uart.h"

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

#define UART_TX_PIN 8
#define UART_RX_PIN 9

void testUart()
{
  // TODO ADD TO PROTOCOL AND SPI TOO
  uart_init(uart1, 115200);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  while (1)
  {

    uart_puts(uart1, "String for uart");
//    sleep_ms(1000);
    std::cout << "str for all\n";
    sleep_ms(1000);
  }
}

int main()
{
  return start_app();
}
