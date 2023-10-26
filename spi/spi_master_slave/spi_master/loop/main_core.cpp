#include "main_core.hpp"
#include <bitset>
#include <iostream>
#include "../utilities/peripheral_functions.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "common_data/common_variables.hpp"
#include "../utilities/debug_logger.hpp"

void MainCore::loop()
{
//  green();
  // remove true and add var
  uint64_t time = 0;
  while (time++ < UINT64_MAX - 1000)
  {
    log(vector, vectorSize);
    // Enable LID while stop command is come to PICO
    if (CONVERGENCE)   //approach
    {
      CONVERGENCE = false;
      blue();
      static uint32_t convergence_data[7];
      convergence_data[0] = vector[1];
      convergence_data[1] = vector[2];
      convergence_data[2] = vector[3];
      convergence_data[3] = vector[4];
      convergence_data[4] = vector[5];
      convergence_data[5] = vector[6];
      convergence_data[6] = vector[7];
      approacphm(convergence_data);
      green();
      sleep_ms(1000);
      dark(); 
    }
    if (LID_UNTIL_STOP)
    {
      moveLinearDriverUntilStop(vector[1], vector[2], vector[3], vector[4], vector[5]);
    }
    // Move scanner to point (x, y)
    if (MOVE_TO)
    {
      MOVE_TO = false;
      scanner.move_to({static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2])}, vector[3]);
      continue;
    }
    // Enable scanner and update config on command 50
    if (MICRO_SCAN || CONFIG_UPDATE)
    {
      if (CONFIG_UPDATE)
      { 
        CONFIG_UPDATE = false;
        scanner.update({static_cast<uint32_t>(vector[1]), static_cast<uint32_t>(vector[2]),
                        static_cast<uint8_t>(vector[3]), static_cast<uint8_t>(vector[4]),
                        static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
                        static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
                        static_cast<uint8_t>(vector[9])});
        continue;
      }
      if (MICRO_SCAN)
      {
        scanner.start_scan();
      }
    }
#warning need to describe thiss block
    if (SET_IO_VALUE)
    {
      SET_IO_VALUE = false;
      if (vector[1] == 1)
      {
        std::string binary = std::bitset<2>(vector[2]).to_string();
        binary[1] == '1' ? io1_0.enable() : io1_0.disable();
        binary[0] == '1' ? io1_1.enable() : io1_1.disable();
      } else if (vector[1] == 2)
      {
        std::string binary = std::bitset<3>(vector[2]).to_string();
        binary[2] == '1' ? io2_0.enable() : io2_0.disable();
        binary[1] == '1' ? io2_1.enable() : io2_1.disable();
        binary[0] == '1' ? io2_2.enable() : io2_2.disable();
      } else if (vector[1] == 3)
      {
        std::string binary = std::bitset<2>(vector[2]).to_string();
        binary[1] == '1' ? io3_0.enable() : io3_0.disable();
        binary[0] == '1' ? io3_1.enable() : io3_1.disable();
      }
    }
    if (SET_ONE_IO_VALUE)
    {
      // #warning Logic replaced
      SET_ONE_IO_VALUE = false;
      vector[2] == 1 ? io_ports[vector[1] - 1].enable() : io_ports[vector[1] - 1].disable();
    }
    // Enable LID
    if (LID)       // piezo mover
    {
      LID = false;
      static uint16_t inBuf[5]; // f, p, n, d
      for (int j = 0; j < 5; ++j)
      {
        inBuf[j] = vector[j];
        std::cout <<inBuf[j] << ' ';
      }
      linearDriver.activate(inBuf[0], inBuf[1], inBuf[2], inBuf[3], inBuf[4]);
      std::cout << "debugLID_IS_READY\n";
      continue;
    }
    // Start scan on ADC
    if (AD7606_IS_SCANNING)       // АЧХ
    {
      static uint16_t inBuf[5]; // n, start_freq, step, channel, delay
      if (!is_already_scanning)
      {
        is_already_scanning = true;
        //afc="code25"; 
        afc="code25,"; //231025
        for (int j = 0; j < 5; ++j)
        {
          inBuf[j] = vector[1 + j];
        }
        current_channel = inBuf[3] - 1;
      } else
      {
        if (scan_index++ < inBuf[0] && !AD7606_STOP_SCAN)
        {
          set_freq(inBuf[1]);
          sleep_ms(inBuf[4]);
          if (!flgVirtual) {get_result_from_adc();} //231025
          else {
               current_freq=6000+10*scan_index;
               afc +=std::to_string(current_freq) + ',' + std::to_string(current_freq)+',';
               //afc +=','+std::to_string(current_freq) + ',' + std::to_string(current_freq);
              }
          sleep_ms(10);
          inBuf[1] += inBuf[2];
        }
        else
        {
          std::cout << afc << '\n';
          afc.clear();
          scan_index = current_freq = 0;
          current_channel = -1;
          is_already_scanning = false;
          AD7606_IS_SCANNING = AD7606_STOP_SCAN = false;
        }
      }
      continue;
    }
    // SET FREQ ON
    if (AD9833_SET_FREQ)
    {
//            set_clock_enable();
      AD9833_SET_FREQ = false;
      set_freq((uint32_t) vector[1]);
    }
    if (AD8400_SET_GAIN)
    {
      AD8400_SET_GAIN = false;
      set_gain(vector[1]);
    }
    if (AD7606_GET_VALUE)
    {
//    set_clock_enable();
      AD7606_GET_VALUE = false;
      AD7606_TRIG_GET_VALUE = true;
      critical_section_enter_blocking(&criticalSection);
      current_channel = vector[1];  // выводить значение CURRENT CHANNEL
      get_result_from_adc();
      continue;
    }
    if (DAC8563_INIT)
    {
      DAC8563_INIT = false;
      dac8563.initialize(vector[1]);
    }

    /// MAIN SPI IF
    decoder.activePort(vector[1]);
    Spi::setProperties(vector[2], vector[3], vector[4]);

    if (DAC8563_SET_VOLTAGE)
    {
      DAC8563_SET_VOLTAGE = false;
      if (vector[5] == 0)
      {
        dac8563.writeA(vector[6]);
      } else if (vector[5] == 1)
      {
        dac8563.writeB(vector[6]);
      }
    }

    if (AD5664)
    {
      AD5664 = false;
      AD56X4Class::setChannel(AD56X4_SETMODE_INPUT, vector[6], vector[5]);
      AD56X4Class::updateChannel(vector[6]);
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
    }
    if (AD8400_SENDER)
    {
      AD8400_SENDER = false;
      uint8_t inBuf[1];
      inBuf[0] = vector[5];
      spi_write_blocking(spi_default, inBuf, 1);
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
    }
    if (AD7606_RESET)
    {
      AD7606_RESET = false;
      resetPort.enable();
      sleep_us(10);
      resetPort.disable();
    }
    if (AD7606_READ or AD7606_READ_FOREVER)
    {
      AD7606_READ = false;
      AD7606_GET_ALL_VALUES = true;
//            set_clock_enable();
      if (AD_7606_IS_READY_TO_READ)
      {
       if (!flgVirtual) { get_result_from_adc();} //231025 
       else
       {
        afc.clear();
        afc="code12,"+ std::to_string(32000)+','+std::to_string(32000)+"\n";   
        std::cout << afc;
        afc.clear();
       }
      }
    }
  }

}

MainCore::MainCore()
{

}
