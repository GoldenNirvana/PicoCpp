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
  uint64_t time = 0;
  while (time++ < UINT64_MAX - 1000)
  {
    //    log(vector, vectorSize);
    if (APPROACH)   //сближение зонда и образца
    {
      blue();
      scanner.approacphm(vector);
      green();
      activateDark();
      continue;
    }
    if (LID_MOVE_UNTIL_STOP)  // пьезодвижитель  позиционирование
    {
      scanner.positioningXYZ(vector); 
      continue;
    }
    if (MOVE_TOX0Y0) //переместиться в начальную точку  скана из начальной точке предыдущего скана
    {
      MOVE_TOX0Y0 = false;
      scanner.move_toX0Y0(static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]), vector[3]);
      continue;
    }

    if (LID_MOVE_TOZ0) //отвестись в безопасную начальную точку по Z
    {
      LID_MOVE_TOZ0 = false;
      scanner.LID_move_toZ0(vector[1], vector[2], vector[3], vector[4], vector[5]);
      continue;
    }
    if (SCANNING) //сканирование
    {  
      if (!scanner.getHoppingFlg())  { scanner.start_scan(vector);       }
      else                           { scanner.start_hopingscan(vector); }
      continue;
    }
    if (FASTSCANNING)
    {  
      FASTSCANNING = false;
      scanner.start_fastscan(vector);  
      continue;
    }
    if (SET_PID_GAIN)         // установить усиление ПИД
    {
      SET_PID_GAIN=false;   
       /* для отладки
      afc.clear();
      afc = "debug PID gain parameters";
      afc += ',' + std::to_string(vector[1]);
      afc += +"\n";
      std::cout << afc;
      sleep_ms(100);
     */
      if (!flgVirtual) set_io_value(vector[1], vector[2]);   
      continue;
    }
    if (SCANNER_RETRACT_PROTRACT) //втянуть-вытянуть сканер
    {
      SCANNER_RETRACT_PROTRACT = false;
      scanner.scanner_retract_protract(vector[1],vector[2]);
     // vector[2] == 1 ? io_ports[vector[1] - 1].enable() : io_ports[vector[1] - 1].disable();
      continue;
    }
    if (LOOP_FREEZE_UNFREEZE) //заморозить- разморозить ПИД ??????????
    {  
      LOOP_FREEZE_UNFREEZE = false;
      scanner.LOOP_freeze_unfreeze(vector[1],vector[2]); 
      //vector[2] == 1 ? io_ports[vector[1] - 1].enable() : io_ports[vector[1] - 1].disable();
      continue;
    }
    // управление пьезодвижителем
    if (LID)       
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

    if (RESONANCE)       //снятие  АЧХ зонда
    {
      scanner.start_frqscan();
      continue;
    }

    if (SPECTROSOPY_IV)
    { 
      SPECTROSOPY_IV=false;
      scanner.spectroscopyIV(vector);
      continue;
    }
    if (SPECTROSOPY_AZ)
    {
      SPECTROSOPY_AZ=false;
      scanner.spectroscopyAZ(vector);
      continue;
    }
    if (FREQ_SET) // установка частоты колебаний зонда
    {
      FREQ_SET = false;
      set_freq((uint32_t) vector[1]);
      continue;
    }
    if (SET_AMPLMOD_GAIN) // усиление раскачка зонда 
    {
      SET_AMPLMOD_GAIN=false;
      set_gain(vector[1]);
      continue;
    } 
  
 /// MAIN SPI 
    if (InitDAC_BIAS_SET_POINT) 
    {
      InitDAC_BIAS_SET_POINT=false; 
      dac8563_1.initialize(vector[1]);
      continue;
    }
    if (InitDAC_XY)
    {
      InitDAC_XY=false; 
      dac8563_2.initialize(vector[1]);
      continue;
    }
    if (SET_BIAS) 
    {
      SET_BIAS=false;
      int16_t v1=vector[1];
      int16_t v2=vector[2];
      int16_t v3=vector[3];
      int16_t v4=vector[4];
      IniSPI(v1,v2,v3,v4);//22, 2, 8, 0, 1, 1, value	
      set_Bias(vector[5],vector[6]);
      continue;
    }
    if (SET_SETPOINT)
    {
      SET_SETPOINT=false;
      int16_t v1=vector[1];
      int16_t v2=vector[2];
      int16_t v3=vector[3];
      int16_t v4=vector[4];
      IniSPI(v1,v2,v3,v4);  //22, 2, 8, 0, 1, 0, value	
      set_SetPoint(vector[5],vector[6]);
      continue;
    }
    if (SET_XY)  //управление  сканером X,Y
    {
      int16_t v1=vector[1];
      int16_t v2=vector[2];
      int16_t v3=vector[3];
      int16_t v4=vector[4];
      IniSPI(v1,v2,v3,v4);//22, 3, 8, 0, 1, 1, value	
      if (vector[5] == 0)
      {
        dac8563_2.writeA(vector[6]);
      } 
      else 
      if (vector[5] == 1)
      {
        dac8563_2.writeB(vector[6]);
      }
      SET_XY=false;
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
    if (ADC_ENABLE_DISABLE)
    {
      ADC_ENABLE_DISABLE = false;
      if (vector[5] == 1)
      {
        ADC_READ_FOREVER = false;
      } else if (vector[5] == 0)
      {
        ADC_READ_FOREVER = true;
      }
      continue;
    }
    if (ADC_RESET)
    {
      ADC_RESET = false;
      resetPort.enable();
      sleep_us(10);
      resetPort.disable();
      continue;
    }
    if (ADC_READ or ADC_READ_FOREVER)   // прочитать АЦП и послать значения на ПК
    {
    //  if (flgDebugLevel <= DEBUG_LEVEL)       logger("ReadADC\n");
      ADC_READ = false;
      if (ADC_IS_READY_TO_READ)
      {
       scanner.readADC();
      }
      continue;
    }
    if (SCANNER_RETRACT)
    {
      scanner.retract();
      SCANNER_RETRACT = false;
      continue;
    }
    if (SCANNER_PROTRACT)
    {
      scanner.protract();
      SCANNER_PROTRACT = false;
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
