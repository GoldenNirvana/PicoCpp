#ifndef PICO_EXAMPLES_SPI_HPP
#define PICO_EXAMPLES_SPI_HPP

#include "hardware/spi.h"

class Spi
{
public:
  Spi();

  static void setProperties(int, int, int, int first = static_cast<spi_order_t>(1));

  static int write(const uint8_t *, size_t length);

  static int read(const uint8_t *inB, uint8_t *buf, size_t length);
};


#endif
