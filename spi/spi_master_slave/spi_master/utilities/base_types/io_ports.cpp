#include "io_ports.h"

InputPort::InputPort(int port) : port_(port)
{
  gpio_init(port);
  gpio_set_dir(port, GPIO_IN);
}

bool InputPort::getLevel() const
{
  return gpio_get(port_);
}

int InputPort::getPort()
{
  return port_;
}


OutputPort::OutputPort(int port) : port_(port)
{
  gpio_init(port);
  gpio_set_dir(port, GPIO_OUT);
}

void OutputPort::enable()
{
  gpio_put(port_, true);
}

void OutputPort::disable()
{
  gpio_put(port_, false);
}

void OutputPort::blink()
{
  gpio_put(port_, true);
  sleep_us(10);
  gpio_put(port_, false);
}

int OutputPort::getPort() const
{
  return port_;
}