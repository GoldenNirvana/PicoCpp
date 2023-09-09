#include "peripheral_functions.hpp"
#include "../loop/common_data/common_variables.hpp"
#include "../transceiver/rx_core.hpp"
#include <pico/multicore.h>

void resetAll()
{
  AD9833_SENDER = false;
  AD8400_SENDER = false;
  AD7606_ENABLE_DISABLE = false;
  AD7606_RESET = false;
  AD7606_READ = false;
  AD7606_READ_FOREVER = false;
  AD7606_IS_SCANNING = false;
}

void setDefaultSettings()
{
  /// BASIC SETTINGS
  gpio_pull_down(resetPort.getPort());
  // fixme mb should add & before isr
  gpio_set_irq_enabled_with_callback(busy.getPort(), GPIO_IRQ_EDGE_FALL, true, comReceiveISR);

  multicore_launch_core1(launchOnCore1);

  dec.enable();
  conv.enable();
  resetPort.disable();
  gpio_pull_down(resetPort.getPort());
  ledPort.enable();
}

void activateError()
{
  while (true)
  {
    ledPort.enable();
    sleep_ms(100);
    ledPort.disable();
    sleep_ms(100);
  }
}