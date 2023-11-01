#include "Spi.hpp"
#include "pico/stdlib.h"
#include <cstdio>

Spi::Spi()
{
  stdio_init_all();
  stdio_usb_init();
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
#warning spi/spi_master example requires a board with SPI pins
  puts("Default SPI pins were not defined");
#else

  printf("SPI master example\n");

  spi_init(spi_default, 1000 * 1000);
  gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);
//  bi_decl(bi_4pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN,
  spi_set_format(spi_default, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

#endif
}

void Spi::setProperties(int bitInWord, int cpol, int chpa, int first)
{
  spi_set_format(spi_default, bitInWord, static_cast<spi_cpol_t>(cpol), static_cast<spi_cpha_t>(chpa),
                 static_cast<spi_order_t>(first));
}

int Spi::write(const uint8_t *buf, size_t length)
{
  return spi_write_blocking(spi_default, buf, length);
}

int Spi::read(const uint8_t *inB, uint8_t *buf, size_t length)
{

  if (spi_is_readable(spi_default))
  {
    spi_write_read_blocking(spi_default, inB, buf, length);
//    spi_read_blocking(spi_default, 0, buf, length);
//    spi_write16_read16_blocking(spi_default, nullptr, buf, length);
  }
    return length;
}
