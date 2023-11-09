#include <cstring>
#include "DAC8563.hpp"

#include "DAC8563.hpp"
#include "../utilities/base_types/Spi.hpp"
#include "../loop/common_data/common_variables.hpp"

DAC8563::DAC8563()
{
  _vref = 5.0;
  begin();
}

void DAC8563::begin()
{
  Spi::setProperties(8, 0, 1);
}


void DAC8563::DAC_WR_REG(uint8_t cmd_byte, uint16_t data_byte)
{
  Spi::setProperties(8, 0, 1);
  uint8_t array[] = {cmd_byte, (uint8_t)(data_byte >> 8),(uint8_t)((data_byte << 8) >> 8)};
  decoder.activePort(port_);
  spi_write_blocking(spi_default, array, 3);
  decoder.activePort(7);
}


void DAC8563::outPutValue(uint8_t cmd_byte, uint16_t input)
{
  uint8_t inputMid = ((input << 8) >> 8);
  uint8_t inputLast = (input >> 8);
//  unsigned int t = (input >> 8) & 0xFF;
  writeValue(cmd_byte, (inputLast), (inputMid));
}

void DAC8563::writeVoltage(int input)
{
  writeA(input);
  writeB(input);
}

void DAC8563::writeA(int input)
{
  outPutValue(CMD_SETA_UPDATEA, input);
}

void DAC8563::writeB(int input)
{
  outPutValue(CMD_SETB_UPDATEB, input);
}

void DAC8563::writeValue(uint8_t cmd_byte, uint8_t mid, uint8_t last)
{
  uint8_t array[] = {cmd_byte, mid, last};
  spi_write_blocking(spi_default, array, 3);
}

void DAC8563::initialize(int port)
{
  port_ = port;
  DAC_WR_REG(CMD_RESET_ALL_REG, DATA_RESET_ALL_REG);      // reset
  DAC_WR_REG(CMD_PWR_UP_A_B, DATA_PWR_UP_A_B);        // power up
  DAC_WR_REG(CMD_INTERNAL_REF_EN, DATA_INTERNAL_REF_EN);      // enable internal reference
  DAC_WR_REG(CMD_GAIN, DATA_GAIN_B1_A1);            // set multiplier
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

void DAC8563::setSpiProps()
{
  Spi::setProperties(8, 0, 1);
}
