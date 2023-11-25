#include "main_core.hpp"
#include <bitset>
#include <iostream>
#include "../utilities/peripheral_functions.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "common_data/common_variables.hpp"
#include "../utilities/debug_logger.hpp"
#include <cmath>

void MainCore::loop()
{
  dark();
  // remove true and add var
  uint64_t time = 0;
  while (time++ < UINT64_MAX - 1000)
  {
//    log(vector, vectorSize);
    // Enable LID while stop command is come to PICO
    if (APPROACH)   //approach
    {
      blue();
      static int16_t approach_data[9];
      approach_data[0] = vector[1]; // point
      approach_data[1] = vector[2]; // max
      approach_data[2] = vector[3]; // min
      approach_data[3] = vector[4]; // steps
      approach_data[4] = vector[5]; // initdelay
      approach_data[5] = vector[6]; // gain
      approach_data[6] = vector[7]; // scannerDelay
      approach_data[7] = vector[8]; // freq
      approach_data[8] = vector[9]; // scv
      scanner.approacphm(approach_data);
      //  sleep_ms(100);
      green();
      activateDark();
      continue;
    }
    if (LID_UNTIL_STOP)  // пьезо мувер позиционирование
    {
      scanner.positioningXYZ(vector[1], vector[2], vector[3], vector[4], vector[5], vector[6],
                             vector[7]); //int lid_name, int f, int p, int n, int dir
     continue;
    }
      // Move scanner to point (x, y)
    if (MOVE_TOX0Y0) //переместиться в начальную точку  скана из начальной точке предыдущего скана
    {
      MOVE_TOX0Y0 = false;
      scanner.move_toX0Y0(static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]), vector[3]);
      continue;
    }

    if (MOVE_TOZ0) //отвестись в безопастную начальную точку по Z
    {
      MOVE_TOZ0 = false;
      scanner.move_toZ0(vector[1], vector[2], vector[3], vector[4], vector[5]);
      continue;
    }

    // Enable scanner and update config on command 50
    if (SCANNING || CONFIG_UPDATE) //scanning
    {
      if (CONFIG_UPDATE)
      {
        CONFIG_UPDATE = false;
        scanner.update({
                        static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]),
                        static_cast<uint8_t>(vector[3]),  static_cast<uint8_t>(vector[4]),
                        static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
                        static_cast<uint16_t>(vector[7]), static_cast<uint8_t>(vector[8]),
                        static_cast<uint8_t>(vector[9]),  static_cast<uint16_t>(vector[10]),
                        static_cast<uint16_t>(vector[11]),static_cast<uint8_t>(vector[12])
                       });  
        continue;
      }
      if (SCANNING)
      {
        scanner.start_scan();
        continue;
      }
    }
    if (FASTSCANNING)
    {
      scanner.update({
                        static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]),
                        static_cast<uint8_t>(vector[3]),  static_cast<uint8_t>(vector[4]),
                        static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
                        static_cast<uint16_t>(vector[7]), static_cast<uint8_t>(vector[8]),
                        static_cast<uint8_t>(vector[9]),  static_cast<uint16_t>(vector[10]),
                        static_cast<uint16_t>(vector[11]),static_cast<uint8_t>(vector[12])
                     });
      scanner.start_fastscan();
      FASTSCANNING=false;
    }
    if (SET_IO_VALUE)
    {
      SET_IO_VALUE = false;
      set_io_value(vector[1], vector[2]);
      continue;
    }
    if (SET_ONE_IO_VALUE)
    {
      SET_ONE_IO_VALUE = false;
      vector[2] == 1 ? io_ports[vector[1] - 1].enable() : io_ports[vector[1] - 1].disable();
      continue;
    }
    // Enable LID
    if (LID)       // piezo mover
    {
      LID = false;
      static uint16_t inBuf[5]; // f, p, n, d
      for (int j = 0; j < 5; ++j)
      {
        inBuf[j] = vector[j];
        if (flgDebugLevel <= DEBUG_LEVEL) std::cout << inBuf[j] << ' ';
      }
      linearDriver.activate(inBuf[0], inBuf[1], inBuf[2], abs(inBuf[3]), inBuf[4]);
      if (flgDebugLevel <= DEBUG_LEVEL) std::cout << "debug LID_IS_READY\n";
      continue;
    }
    // Start scan on ADC
    if (RESONANCE)       // АЧХ
    {
      scanner.start_frqscan();
      continue;
    }
    // SET FREQ ON
    if (FREQ_SET) // установка частоты
    {
//            set_clock_enable();
      FREQ_SET = false;
      set_freq((uint32_t) vector[1]);
      continue;
    }
    if (AD8400_SET_GAIN) // усиление раскачка зонда
    {
      AD8400_SET_GAIN = false;
      set_gain(vector[1]);
      continue;
    }

    if (DAC8563_INIT_1)
    {
      DAC8563_INIT_1 = false;
      dac8563_1.initialize(vector[1]);
      continue;
    }
    if (DAC8563_INIT_2)
    {
      DAC8563_INIT_2 = false;
      dac8563_2.initialize(vector[1]);
      continue;
    }

    /// MAIN SPI IF
    decoder.activePort(vector[1]);
    Spi::setProperties(vector[2], vector[3], vector[4]);

    if (DAC8563_SET_VOLTAGE_1)
    {
      DAC8563_SET_VOLTAGE_1 = false;
      if (vector[5] == 0)
      {
        dac8563_1.writeA(vector[6]);
      } else if (vector[5] == 1)
      {
        dac8563_1.writeB(vector[6]);
      }
      continue;
    }

    if (DAC8563_SET_VOLTAGE_2)
    {
      DAC8563_SET_VOLTAGE_2 = false;
      if (vector[5] == 0)
      {
        dac8563_2.writeA(vector[6]);
      } else if (vector[5] == 1)
      {
        dac8563_2.writeB(vector[6]);
      }
      continue;
    }

    if (AD5664)
    {
      AD5664 = false;
      AD56X4Class::setChannel(AD56X4_SETMODE_INPUT, vector[6], vector[5]);
      AD56X4Class::updateChannel(vector[6]);
      continue;
    }
    if (AD9833_SENDER)
    {
      AD9833_SENDER = false;
      uint8_t buf[6];
      for (int j = 0; j < 6; ++j)
      {
        buf[j] = vector[5 + j];
      }
      spi_write_blocking(spi_default, buf, 6);
      continue;
    }
    if (AD8400_SENDER)
    {
      AD8400_SENDER = false;
      uint8_t inBuf[1];
      inBuf[0] = vector[5];
      spi_write_blocking(spi_default, inBuf, 1);
      continue;
    }
    if (AD7606_ENABLE_DISABLE)
    {
      AD7606_ENABLE_DISABLE = false;
      if (vector[5] == 1)
      {
        AD7606_READ_FOREVER = false;
      } else if (vector[5] == 0)
      {
        AD7606_READ_FOREVER = true;
      }
      continue;
    }
    if (AD7606_RESET)
    {
      AD7606_RESET = false;
      resetPort.enable();
      sleep_us(10);
      resetPort.disable();
      continue;
    }
    if (AD7606_READ or AD7606_READ_FOREVER)   // read ADC
    {
      if (flgDebugLevel <= DEBUG_LEVEL)
        logger("ReadADC\n");
      AD7606_READ = false;
      if (AD_7606_IS_READY_TO_READ)
      {
        afc.clear();
        afc = "code12";
        if (!flgVirtual)
        {
          getValuesFromAdc();
          auto ptr = getValuesFromAdc();
          logger(ptr, 8);
          ZValue = (int16_t) ptr[ZPin];
          SignalValue = (int16_t) ptr[SignalPin];
          set_io_value(2, vector[1]);   //add 231114 gain pid
          afc +=
              ',' + std::to_string(ZValue) + ',' + std::to_string(SignalValue) + ',' + std::to_string(vector[1]) + "\n";
          std::cout << afc;
          afc.clear();
        } else
        {
          afc += ',' + std::to_string(ZValue) + ',' + std::to_string(SignalValue) + "\n";   //Z,Signal
          std::cout << afc;
          afc.clear();
        }
      }
      continue;
    }
    if (SET_PID_GAIN)
    {
      SET_PID_GAIN = false;
      sleep_ms(200);
      afc.clear();
      afc = "debug PID gain parameters";
      afc += ',' + std::to_string(vector[1]);
      afc += +"\n";
      std::cout << afc;
      sleep_ms(100);
      if (!flgVirtual) set_io_value(2, vector[1]);
      continue;
    }
    if (Scanner_Retract)
    {
      scanner.retract();
      Scanner_Retract = false;
      // set_io_value(5,vector[1],vector[2]);
      continue;
    }
    if (Scanner_Protract)
    {
      scanner.protract();
      Scanner_Protract = false;
      // set_io_value(5,vector[1],vector[2]);
      continue;
    }
    if (GET_CURRENTX0Y0)
    {
      scanner.getX0Y0();
      GET_CURRENTX0Y0 = false;
      continue;
    }
  }
}

MainCore::MainCore()
{

}
