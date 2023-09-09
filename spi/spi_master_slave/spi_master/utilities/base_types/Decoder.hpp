#ifndef PICO_EXAMPLES_DECODER_HPP
#define PICO_EXAMPLES_DECODER_HPP

#include "io_ports.h"


class Decoder
{
public:
  Decoder(int a, int b, int c);

  void activePort(int port = 0);

private:
  OutputPort portA;
  OutputPort portB;
  OutputPort portC;
};

#endif
