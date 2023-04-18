#ifndef PICO_EXAMPLES_UTILITIES_HPP
#define PICO_EXAMPLES_UTILITIES_HPP

static uint16_t inputBuf[8];
volatile bool ADC = true;

void serialPrintBuffer(const uint16_t *const buf, int len)
{
  u_int64_t x = time_us_64();
  printf("% ", static_cast<bool>(x));
  for (int i = 0; i < len; ++i)
  {
    printf("%u ", buf[i]);
  }
  printf("\n");
}

void core1()
{
  GpioPort conv(7);
  while (true)
  {
    conv.enable();
    while (ADC)
    {
      conv.disable();
      sleep_us(10);
      conv.enable();
      sleep_us(500); // todo
      spi_read16_blocking(spi_default, 0, inputBuf, 8);
      serialPrintBuffer(inputBuf, 8);
      sleep_ms(1);
    }
  }
}

#endif
