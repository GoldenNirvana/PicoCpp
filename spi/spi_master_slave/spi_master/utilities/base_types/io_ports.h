#ifndef PICO_EXAMPLES_IO_PORTS_H
#define PICO_EXAMPLES_IO_PORTS_H

#include "pico/stdlib.h"


class InputPort
{
public:
  explicit InputPort(int port);

  bool getLevel() const;

  int getPort();

private:
  int port_;
};

class OutputPort
{
public:
  explicit OutputPort(int port);

  void enable();

  void disable();

  void blink();

  int getPort() const;

private:
  int port_;
};


#endif
