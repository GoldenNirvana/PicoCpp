#ifndef PICO_EXAMPLES_DAC8563_HPP
#define PICO_EXAMPLES_DAC8563_HPP

#include <cstdint>

#define CMD_SETA_UPDATEA          0x18  //
#define CMD_SETB_UPDATEB          0x19  //
#define CMD_UPDATE_ALL_DACS       0x0F  //
#define CMD_GAIN                  0x02  //
#define DATA_GAIN_B2_A2           0x0000  //
#define DATA_GAIN_B2_A1           0x0001  //
#define DATA_GAIN_B1_A2           0x0002  //
#define DATA_GAIN_B1_A1           0x0003  //

#define CMD_PWR_UP_A_B            0x20  //
#define DATA_PWR_UP_A_B           0x0003  // Power up DAC-A and DAC-B  data

#define CMD_RESET_ALL_REG         0x28  //
#define DATA_RESET_ALL_REG        0x0001  //

#define CMD_LDAC_DIS              0x30  //
#define DATA_LDAC_DIS             0x0003  //

#define CMD_INTERNAL_REF_DIS      0x38  // Disable internal reference and reset DACs to gain = 1
#define DATA_INTERNAL_REF_DIS     0x0000  // Disable internal reference and reset DACs to gain = 1
#define CMD_INTERNAL_REF_EN       0x38  // Enable Internal Reference & reset DACs to gain = 2
#define DATA_INTERNAL_REF_EN      0x0001  // Enable Internal Reference & reset DACs to gain = 2


class DAC8563
{
private:
  uint8_t _sck_pin;
  uint8_t _mosi_pin;
  uint8_t _miso_pin;
  uint8_t _cs_pin;
  float   _vref;

public:

  DAC8563();
  void begin();
  void initialize();
  void DAC_WR_REG(uint8_t cmd_byte, uint16_t data_byte );
  void outPutValue(uint8_t cmd_byte,uint16_t input);
  void writeValue( uint8_t cmd_byte, uint8_t mid, uint8_t last);
  void writeVoltage(float input);
  void writeA(float input);
  void writeB(float input);
  uint16_t Voltage_Convert(float voltage);
};


#endif
