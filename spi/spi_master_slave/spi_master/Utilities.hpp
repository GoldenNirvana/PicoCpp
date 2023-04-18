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

}

void parse(int *vector)
{
  char msg[BUF_LEN];
  scanf("%20s", msg);
  Parser parser(msg, ',');
  for (int c = 0; c < 10; c++)
  {
    vector[c] = -1;
  }
  parser.parseInts(vector);
  for (int i = 0; i < 8; ++i)
  {
    inputBuf[i] = 0;
  }
}

#endif
