#include "Decoder.hpp"
#include <bitset>

Decoder::Decoder(int a, int b, int c) : portA(GpioPort(a)), portB(GpioPort(b)), portC(GpioPort(c))
{}

void Decoder::activePort(int port)
{
  std::string binary = std::bitset<3>(port).to_string();

  binary[0] == '1' ? portC.enable() : portC.disable();
  binary[1] == '1' ? portB.enable() : portB.disable();
  binary[2] == '1' ? portA.enable() : portA.disable();
}

