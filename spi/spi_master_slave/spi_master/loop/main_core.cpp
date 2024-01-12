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
    switch (ALGCODE)
    {
 
case RESONANCE:
              {
               ALGCODE=0;
               scanner.start_frqscan();
               break;
              }
case APPROACH:{
               ALGCODE=0;
               blue();
               scanner.approacphm(vector);
               green();
               activateDark();
               break;
              }
case FREQ_SET:{
               ALGCODE=0;
               set_freq((uint32_t) vector[1]);              
               break;
              }
case LID_MOVE_UNTIL_STOP:
              {
               ALGCODE=0;
               scanner.positioningXYZ(vector);               
               break;
              }
case MOVE_TOX0Y0:
              {
                ALGCODE=0;
                scanner.move_toX0Y0(vector[1], vector[2], vector[3]);
                break; 
              }     
case LID_MOVE_TOZ0:
              {
                ALGCODE=0;
                scanner.LID_move_toZ0(vector[1], vector[2], vector[3], vector[4], vector[5]);
                break; 
              }            
case SCANNING:
              {
                ALGCODE=0;
                if (!scanner.getHoppingFlg())  { scanner.start_scan(vector);       }
                else                           { scanner.start_hopingscan(vector); }
                break; 
              }
case FASTSCANNING:
              {
                ALGCODE=0;
                scanner.start_fastscan(vector);  
                break; 
              }   

case SET_PID_GAIN:
              {
                ALGCODE=0;
                 if (!flgVirtual) set_io_value(vector[1], vector[2]);     
                break; 
              }     
case InitDAC_BIAS_SET_POINT:
              {
                ALGCODE=0;
                 if (!flgVirtual)  init_DACSPB(vector[1]);       
                break;         
              }   
case InitDAC_XY:
              {
                ALGCODE=0; 
                init_DACXY(vector[1]);
                break; 
              }  
case SET_SETPOINT:
              {
                ALGCODE=0;
                IniSPI(vector[1],vector[2],vector[3],vector[4]);  //22, 2, 8, 0, 1, 0, value	
                set_SetPoint(vector[5],vector[6]);
                break; 
              }     
case SET_XY: {
              ALGCODE=0;
              IniSPI(vector[1],vector[2],vector[3],vector[4]);//22, 3, 8, 0, 1, 1, value	
              if (vector[5] == 0)
              {
               move_scannerX(vector[6]);
               // dac8563_2.writeA(vector[6]);
              } 
              else 
              if (vector[5] == 1)
              {
               move_scannerY(vector[6]);
               // dac8563_2.writeB(vector[6]);
              }
              break;
             }   
case ADC_READ:{
               ALGCODE=0;
               if (ADC_IS_READY_TO_READ)
               {
                scanner.readADC();
               }
               break;
              }    
case GET_CURRENTX0Y0:
              {
                ALGCODE=0;
                scanner.getX0Y0();
                break;
              }   
case SCANNER_RETRACT_PROTRACT:
              {
                ALGCODE=0;
                scanner.scanner_retract_protract(vector[1],vector[2]);
                // vector[2] == 1 ? io_ports[vector[1] - 1].enable() : io_ports[vector[1] - 1].disable();
                break;
              } 
 case SPECTROSOPY_IV:
              {
                ALGCODE=0;
                scanner.spectroscopyIV(vector);
                break;
              }  
 case SPECTROSOPY_AZ:
              {
                ALGCODE=0;
                scanner.spectroscopyAZ(vector);
                break;
              }  
case SET_BIAS:
              {
                ALGCODE=0;
                IniSPI(vector[1],vector[2],vector[3],vector[4]);//22, 2, 8, 0, 1, 1, value	
                set_Bias(vector[5],vector[6]);
                break;
              }                                                                                                                                
   }
  }

/*
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

    if (SET_AMPLMOD_GAIN) // усиление раскачка зонда 
    {
      SET_AMPLMOD_GAIN=false;
      set_gain(vector[1]);
      continue;
    } 
  
 /// MAIN SPI 
 
  

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
     */
}

MainCore::MainCore()
{

}
