#include "main_core.hpp"
#include <bitset>
#include <iostream>
#include "../utilities/peripheral_functions.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "common_data/common_variables.hpp"
#include "../utilities/debug_logger.hpp"
#define _USE_MATH_DEFINES // for C++
#include <cmath>

void MainCore::loop()
{
  green();
  // remove true and add var
  uint64_t time = 0;
  while (time++ < UINT64_MAX - 1000)
  {
  //  log(vector, vectorSize);
    // Enable LID while stop command is come to PICO
    if (APPROACH)   //approach
    {
      blue();
      static int16_t convergence_data[7]; //32???
      convergence_data[0] = vector[1];
      convergence_data[1] = vector[2];
      convergence_data[2] = vector[3];
      convergence_data[3] = vector[4];
      convergence_data[4] = vector[5];
      convergence_data[5] = vector[6];
      convergence_data[6] = vector[7];
      
      convergence_data[7] = vector[8];  // freq  //231102 ????
      convergence_data[8] = vector[9];   // scv ????

      scanner.approacphm(convergence_data);
      
      green();
      sleep_ms(100);
      dark(); 
    }
    if (LID_UNTIL_STOP)  // пьезо мувер позиционирование
    {
      green();     
      scanner.positioningXYZ(vector[1], vector[2], vector[3], vector[4], vector[5],vector[6], vector[7]); //int lid_name, int f, int p, int n, int dir
      //blue();
     // LID_UNTIL_STOP=false; //add
    }
    // Move scanner to point (x, y)
    if (MOVE_TO)
    {
      MOVE_TO = false;
      scanner.move_to({static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2])}, vector[3]);
      continue;
    }
    // Enable scanner and update config on command 50
    if (SCANNING || CONFIG_UPDATE) //scanning
    {
      if (CONFIG_UPDATE)
      {
        CONFIG_UPDATE = false;
        red();
        scanner.update({static_cast<uint32_t>(vector[1]), static_cast<uint32_t>(vector[2]),
                        static_cast<uint8_t>(vector[3]), static_cast<uint8_t>(vector[4]),
                        static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
                        static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
                        static_cast<uint8_t>(vector[9])});
        dark();
        continue;
      }
      if (SCANNING)
      {  
       scanner.start_scan();
      }
    }
#warning need to describe thiss block
    if (SET_IO_VALUE) //setup pin   ports1,2,3
    {
      SET_IO_VALUE = false;
      set_io_value(vector[1], vector[2]);
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
        if (flgDebugLevel<=DEBUG_LEVEL) std::cout <<inBuf[j] << ' ';
      }
      linearDriver.activate(inBuf[0], inBuf[1], inBuf[2], inBuf[3], inBuf[4]);
       if (flgDebugLevel<=DEBUG_LEVEL) std::cout << "debug LID_IS_READY\n";
      continue;
    }
    // Start scan on ADC
    if (RESONANCE)       // АЧХ
    {
      scanner.start_frqscan();
   /*   static uint16_t inBuf[5]; // n, start_freq, step, channel, delay
      int16_t signalvalue; 
      int16_t res_freq=10000;
      int16_t a=10000;
      if (!RESONANCE_ACTIVE)
      {
        RESONANCE_ACTIVE = true;
        afc="code25";//,"; //231025
        for (int j = 0; j < 5; ++j)
        {
          inBuf[j] = vector[1 + j];
        }
        current_channel = inBuf[3] - 1;  //ampl=0;
      }
      else
      {
        if (scan_index++ < inBuf[0] && !RESONANCE_STOP)
        {
      //    blue();
          if (!flgVirtual) 
          {  
            set_freq(inBuf[1]);
            sleep_ms(inBuf[4]);
            afc +=','+ std::to_string(inBuf[1]) + ',' + std::to_string(getValuesFromAdc()[current_channel]);//+ ',';
          } 
          else
          {
            current_freq=vector[2]+inBuf[2]*scan_index;
            signalvalue=std::round(a*(pow(M_E,-pow((current_freq-res_freq),2)/1000000)));
            afc +=','+std::to_string(current_freq) + ',' + std::to_string(signalvalue);//+',';
          }
          sleep_ms(10);
          inBuf[1] += inBuf[2];
        }
        else
        {
          green();
          sleep_ms(500);
          dark();
          std::cout << afc << '\n';
          afc.clear();
          scan_index = current_freq = 0;
          current_channel = -1;
          RESONANCE_ACTIVE = false;
          RESONANCE = RESONANCE_STOP = false;
        }
      }
      continue;
      */
   }
    // SET FREQ ON
    if (FREQ_SET) // установка частоты
    {
//            set_clock_enable();
      FREQ_SET = false;
      set_freq((uint32_t) vector[1]);
    }
    if (AD8400_SET_GAIN) // усиление раскачка зонда
    {
      AD8400_SET_GAIN = false;
      set_gain(vector[1]);
    }
  if (AD7606_GET_VALUE) // get value channel vector[1] 
    {
      AD7606_GET_VALUE = false;
      afc="code24,"; //???
     if (!flgVirtual) 
     {    
       afc+=std::to_string((int16_t)getValuesFromAdc()[SignalPin])+','+std::to_string((int16_t)getValuesFromAdc()[ZPin])+'\n'; //ampl,Z
       std::cout<<afc;    
     }
     else
     {
       afc+=std::to_string(32000)+','+std::to_string(32000)+ '\n'; //ampl,Z
       std::cout<<afc;    
     }
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
    if (AD7606_READ or AD7606_READ_FOREVER)   // read ADC
    {
      if (flgDebugLevel<=DEBUG_LEVEL) log("ReadADC\n",flgDebugLevel);
      AD7606_READ = false;
      if (AD_7606_IS_READY_TO_READ)
      {
        afc.clear();
        afc="code12";
       if (!flgVirtual) 
       {
         auto ptr = getValuesFromAdc(); ///???
              ZValue=(int16_t)ptr[ZPin];
         SignalValue=(int16_t)ptr[SignalPin];
         afc+=','+std::to_string(ZValue)+','+std::to_string(SignalValue)+"\n";
         std::cout<<afc;
         afc.clear();
       }
       else
       {
         afc+=','+std::to_string(ZValue)+','+std::to_string(SignalValue)+"\n";   //Z,Signal
         std::cout << afc;
         afc.clear();
       }
      }
    }
    if (SET_PID_GAIN)
    {
      SET_PID_GAIN=false;
      set_io_value(2,vector[1]);
    }
    if (Scanner_Retract)
    {
      scanner.retract();
      Scanner_Retract=false;
     // set_io_value(5,vector[1],vector[2]);
    }
    if (Scanner_Protract)
    {
      scanner.protract();
      Scanner_Protract=false;
     // set_io_value(5,vector[1],vector[2]);
    }
  }
}

MainCore::MainCore()
{

}
