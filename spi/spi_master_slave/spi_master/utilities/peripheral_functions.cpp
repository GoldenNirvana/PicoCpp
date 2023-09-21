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

#warning should be undeleted
//  RX_core rxCore;

  // fixme mb should add & before isr
  gpio_set_irq_enabled_with_callback(busy.getPort(), GPIO_IRQ_EDGE_FALL, true, RX_core::comReceiveISR);

  multicore_launch_core1(RX_core::launchOnCore1);

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

void _delay_us(double __us)
{
  uint32_t __count=(uint32_t)(__us/0.008)-3; // 8ns per cycle for 125MHz, from experimentation remove 3cycles for overhead
  busy_wait_at_least_cycles(__count);
}

void activateGreen() {

    for (int i = 0; i < 8; ++i) {
        rdbLed.enable();
        busy_wait_at_least_cycles(85);
        rdbLed.disable();
        busy_wait_at_least_cycles(35);
    }

    for (int i = 0; i < 16; ++i) {
        rdbLed.enable();
        busy_wait_at_least_cycles(35);
        rdbLed.disable();
        busy_wait_at_least_cycles(85);
    }
}

void activateRed() {
    for (int i = 0; i < 8; ++i) {
        rdbLed.enable();
        busy_wait_at_least_cycles(35);
        rdbLed.disable();
        busy_wait_at_least_cycles(85);
    }

    for (int i = 0; i < 8; ++i) {
        rdbLed.enable();
        busy_wait_at_least_cycles(85);
        rdbLed.disable();
        busy_wait_at_least_cycles(35);
    }

    for (int i = 0; i < 8; ++i) {
        rdbLed.enable();
        busy_wait_at_least_cycles(35);
        rdbLed.disable();
        busy_wait_at_least_cycles(85);
    }
}

void activateBlue() {

    for (int i = 0; i < 16; ++i) {
        rdbLed.enable();
        busy_wait_at_least_cycles(35);
        rdbLed.disable();
        busy_wait_at_least_cycles(85);
    }
    for (int i = 0; i < 8; ++i) {
        rdbLed.enable();
        busy_wait_at_least_cycles(85);
        rdbLed.disable();
        busy_wait_at_least_cycles(35);
    }
}

void activateDark()
{
    for (int i = 0; i < 24; ++i) {
        rdbLed.enable();
        busy_wait_at_least_cycles(35);
        rdbLed.disable();
        busy_wait_at_least_cycles(85);
    }
}
