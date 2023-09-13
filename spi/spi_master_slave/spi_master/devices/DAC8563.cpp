#include "DAC8563.hpp"

#include "DAC8563.hpp"
#include "../utilities/base_types/Spi.hpp"

DAC8563::DAC8563()
{
  _vref = 3.3383;
}

void DAC8563::begin()
{
  Spi::setProperties(8, 0, 1, SPI_MSB_FIRST);
  initialize();
}


void DAC8563::DAC_WR_REG(uint8_t cmd_byte, uint16_t data_byte)
{
  // check is it differtent ways to send
  static uint8_t array[] = {cmd_byte, (uint8_t) (data_byte >> 8), (uint8_t) ((data_byte << 8) >> 8)};
  Spi::write(array, 3);
}


void DAC8563::outPutValue(uint8_t cmd_byte, uint16_t input)
{
  uint8_t inputMid = (input >> 8) & 0xFF;
  uint8_t inputLast = input & 0xFF;
  unsigned int t = (input >> 8) & 0xFF;
  writeValue(cmd_byte, (inputLast), (inputMid));
}

void DAC8563::writeVoltage(float input)
{
  writeA(input);
  writeB(input);
}

void DAC8563::writeA(float input)
{
  outPutValue(CMD_SETA_UPDATEA, Voltage_Convert(input / _vref * 5));
}

void DAC8563::writeB(float input)
{
  outPutValue(CMD_SETB_UPDATEB, Voltage_Convert(input / _vref * 5));
}

void DAC8563::writeValue(uint8_t cmd_byte, uint8_t mid, uint8_t last)
{
  static uint8_t array[] = {cmd_byte, mid, last};
  Spi::write(array, 3);
}

void DAC8563::initialize()
{
  DAC_WR_REG(CMD_RESET_ALL_REG, DATA_RESET_ALL_REG);      // reset
  DAC_WR_REG(CMD_PWR_UP_A_B, DATA_PWR_UP_A_B);        // power up
  DAC_WR_REG(CMD_INTERNAL_REF_EN, DATA_INTERNAL_REF_EN);      // enable internal reference
  DAC_WR_REG(CMD_GAIN, DATA_GAIN_B2_A2);            // set multiplier
  DAC_WR_REG(CMD_LDAC_DIS, DATA_LDAC_DIS);          // update the caches
}

uint16_t DAC8563::Voltage_Convert(float voltage)
{
  uint16_t _D_;

  voltage = voltage / 6 + 2.5;   //based on the manual provided by texas instruments

  _D_ = (uint16_t) (65536 * voltage / 5);

  if (_D_ < 32768)
  {
    _D_ -= 100;     //fix the errors
  }

  return _D_;
}