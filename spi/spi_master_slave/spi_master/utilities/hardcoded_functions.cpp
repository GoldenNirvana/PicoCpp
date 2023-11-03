#include "hardcoded_functions.hpp"
#include "../loop/common_data/common_variables.hpp"
#include "../transceiver/rx_core.hpp"
#include <pico/multicore.h>
#include <iostream>
#include "../utilities/debug_logger.hpp"
#include "peripheral_functions.hpp"


void set_freq(uint32_t freq)
{
  current_freq = freq;

  int64_t flag_freq = 1 << 14;
  int64_t scale = 1 << 28;
  int64_t n_reg = int64_t(freq * scale / 25.0e6);
  int64_t n_low = n_reg & 0x3fff;
  int64_t n_hi = (n_reg >> 14) & 0x3fff;

  uint8_t buf[6];

  buf[0] = (flag_freq | n_low) / (0x100);
  buf[1] = (flag_freq | n_low) % (0x100);
  buf[2] = (flag_freq | n_hi) / (0x100);
  buf[3] = (flag_freq | n_hi) % (0x100);
  buf[4] = (0x2000) / (0x100);
  buf[5] = (0x2000) % (0x100);

//  serialPrintBuffer(buf, 6);

  decoder.activePort(1);
  Spi::setProperties(8, 1, 1);
  spi_write_blocking(spi_default, buf, 2);
  spi_write_blocking(spi_default, buf + 2, 2);
  spi_write_blocking(spi_default, buf + 4, 2);

}

void get_result_from_adc()
{
  AD_7606_IS_READY_TO_READ = false;
  conv.disable();
  sleep_us(10);
  conv.enable();
}

void set_gain(int gain, int p)
{
  uint8_t intBuf[1];
  decoder.activePort(p);
  Spi::setProperties(8, 0, 0);
  intBuf[0] = gain;
  spi_write_blocking(spi_default, intBuf, 3);
}

void set_clock_enable()
{
  uint8_t intBuf[1];
  Spi::setProperties(8, 1, 1);
  decoder.activePort(7);
  spi_write_blocking(spi_default, intBuf, 1);
}

void set_on_cap(uint8_t channel, uint16_t value)
{
  decoder.activePort(3);
  dac8563.setSpiProps();
//  AD56X4Class::setChannel(AD56X4_SETMODE_INPUT, channel, value);
//  AD56X4Class::updateChannel(channel);
  channel--;
  if (channel == 0)  dac8563.writeA(value);
  if (channel == 1)  dac8563.writeB(value);
}

void stopAll()
{
  AD9833_SENDER = false;
  AD8400_SENDER = false;
  AD7606_ENABLE_DISABLE = false;
  AD7606_RESET = false;
  AD7606_READ = false;
  AD7606_READ_FOREVER = false;
  AD7606_STOP_SCAN = true;
  AD9833_SET_FREQ = false;
  AD8400_SET_GAIN = false;
  AD7606_GET_VALUE = false;
  AD5664 = false;
  MICRO_SCAN = false;
  CONVERGENCE = false;
  CONFIG_UPDATE = false;
  MOVE_TO = false;
  STOP_ALL = true;
  SET_IO_VALUE = false;
  SET_ONE_IO_VALUE = false;
  LID = false;
  DAC8563_SET_VOLTAGE = false;
  DAC8563_INIT = false;
  LID_UNTIL_STOP = false;
  AD_7606_IS_READY_TO_READ = true;
  AD7606_IS_SCANNING = false;
  is_already_scanning = false;
  scan_index = 0;
  current_freq = 0;
  afc.clear();
}

uint16_t *getValuesFromAdc()
{
  get_result_from_adc();
  int i = 0;
  while (!AD_7606_IS_READY_TO_READ && i++ < 3)
  {
//    std::cout << "EndlessLoop\n";
    sleep_us(100);
  }
  return spiBuf;
}


void approacphm(const uint32_t *const data)
{
 // const int waitsteps = 40;
  const int stopdone=5;
  const int none = 30;
  uint32_t SIGNAL,Z0, Z;
  uint32_t SET_POINT, GATE_Z_MAX, GATE_Z_MIN;
  int32_t  GAIN, NSTEPS;
  uint32_t INTDELAY, SCANNERDECAY;

  // SET VALUE FROM RX_CORE
    SET_POINT  = data[0];
    GATE_Z_MAX = data[1];
    GATE_Z_MIN = data[2];
        NSTEPS = data[3];
      INTDELAY = data[4];
          GAIN = data[5];
  SCANNERDECAY = data[6];
//  freq = data[7];
 // scv = data[8];

  dac8563.writeA(SET_POINT);
  std::vector<uint32_t> buf_params;
  buf_params.reserve(7);
  for (int i = 0; i < 7; ++i)
    buf_params.push_back(data[i]);

 if (!flgVirtual) //add mf
  {
    getValuesFromAdc();  
    uint16_t *ptr = getValuesFromAdc();
    SIGNAL = (int16_t) ptr[0];
    Z = (int16_t) ptr[1];
  }
  else
  { 
     SIGNAL = 32000;
         Z0 = 32000;
         Z  = Z0;
  }  

  std::vector<uint32_t> buf_status;
  buf_status.push_back(none);
  buf_status.push_back(Z);
  buf_status.push_back(SIGNAL);

  while (true)
  {
    buf_status[0] = none;
    if (!CONVERGENCE)
    {
      red();
      sleep_ms(500);
      dark();
      buf_status[0] = stopdone;
      buf_status[1] = Z;
      buf_status[2] = SIGNAL;
      break;
    }
     if (CONVERGENCE_CONFIG_UPDATE)
    {
      green();
      sleep_ms(100);
      dark();
     log("config updated\n",flgDebugLevel);
      CONVERGENCE_CONFIG_UPDATE = false; //add 231025 ???? Ilia
      SET_POINT  = vector[1];
      GATE_Z_MAX = vector[2];
      GATE_Z_MIN = vector[3];
      NSTEPS     = vector[4];
      INTDELAY   = vector[5];
      GAIN       = vector[6];
    SCANNERDECAY = vector[7]; 
    }

    dac8563.writeA(SET_POINT);
    set_io_value(2, GAIN);   
    sleep_ms(INTDELAY);

   if (!flgVirtual) 
   {
   //  getValuesFromAdc(); 
     /*uint16_t *ptr = getValuesFromAdc();  
     Z = (int16_t) ptr[1]; 
     SIGNAL = (int16_t) ptr[0];
     */
     Z=getValuesFromAdc()[1];
     SIGNAL=getValuesFromAdc()[0];
     if (flgDebugLevel<=DEBUG_LEVEL) log("Z = " + std::to_string(Z) + '\n',flgDebugLevel);
   }
   else
   { 
     SIGNAL = 32000;
     if (NSTEPS >= 0)
     {
      Z = Z-500;
     } 
     else
     {       
       if ((Z0-Z)>0){Z+=500;}
       else {Z=Z0;}
     }  
     buf_status[1] = Z;
     buf_status[2] = SIGNAL;
   }  
    if (NSTEPS >= 0)
    {
      if (Z <= GATE_Z_MIN)
      {
        buf_status[0] = 2; // touch
        buf_status[1] = Z;
        buf_status[2] = SIGNAL;
        break;
      }
      if (Z <= GATE_Z_MAX)
      {
        int k = 0;
        for (int i = 0; i < 10; ++i)
        {
          if (!flgVirtual) 
           {
            //  getValuesFromAdc();
            //  auto pt = getValuesFromAdc();
             Z=getValuesFromAdc()[1];
           }
          if (Z <= GATE_Z_MAX)    k++;
          if (k == 3)
          {
           buf_status[0] = 3; // ok
           buf_status[1] = Z;
           buf_status[2] = SIGNAL;
           if (flgDebugLevel<=DEBUG_LEVEL) log("success\n",flgDebugLevel);
           break;
          }
          sleep_ms(10);
        }
        if (buf_status[0] == 3) {break;}
      }
    } //NSTEPS>0
    if (NSTEPS < 0)
    {
    }
      afc.clear(); 
      afc= "code75," + std::to_string(buf_status[0])+',' +std::to_string(buf_status[1]) +',' +std::to_string(buf_status[2])+"\n";  
      std::cout << afc;
      afc.clear();
    if (!flgVirtual) 
    {
     io3_1.enable(); // втянуть 231025
     sleep_ms(SCANNERDECAY);
     linearDriver.activate(99, 5000, 750, std::abs(NSTEPS), NSTEPS > 0);
     io3_1.disable(); //вытянуть
    }
  }
      afc.clear(); 
      afc= "code75," + std::to_string(buf_status[0])+',' +std::to_string(buf_status[1]) +',' +std::to_string(buf_status[2])+"\n";  
      std::cout << afc;
      afc.clear();   
      CONVERGENCE = false; 
      io3_1.disable();//вытянуть
      sleep_ms(INTDELAY); 
      std::cout << "end\n";
}
