#ifndef PICO_EXAMPLES_DECODER_HPP
#define PICO_EXAMPLES_DECODER_HPP

#include "pico/stdlib.h"

class InputPort
{
public:
  explicit InputPort(int port) : port_(port)
  {
    gpio_init(port);
    gpio_set_dir(port, GPIO_IN);
  }

  bool getLevel() const
  {
    return gpio_get(port_);
  }

  int getPort()
  {
      return port_;
  }

private:
  int port_;
};

class GpioPort
{
public:
  explicit GpioPort(int port) : port_(port)
  {
    gpio_init(port);
    gpio_set_dir(port, GPIO_OUT);
  }

  void enable() const
  {
    gpio_put(port_, true);
  }

  void disable() const
  {
    gpio_put(port_, false);
  }

  void blink() const
  {
    gpio_put(port_, true);
    sleep_us(10);
    gpio_put(port_, false);
  }

  int getPort() const
  {
    return port_;
  }

private:
  int port_;
};

class Decoder
{
public:
  Decoder(int a, int b, int c);

  void activePort(int port = 0);

private:
  GpioPort portA;
  GpioPort portB;
  GpioPort portC;
};

#endif
