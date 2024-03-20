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
case   ALGNONE:{break;} //????
case RESONANCE:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);
               scanner.start_frqscan();
               break;
              }
case APPROACH:{
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);
               blue();
               scanner.approacphm(vector);
               //green();
               dark();
               break;
              }
case TESTMOVER:{
                critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);
               blue();
               scanner.testpiezomover(vector);
               //green();
               dark();
               break;
              }
case FREQ_SET:{
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);
               set_Freq((uint32_t) vector[1]);              
               break;
              }
case LID_MOVE_UNTIL_STOP:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);
               scanner.positioningXYZ(vector);               
               break;
              }
case MOVE_TOX0Y0:
              {
                critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
                critical_section_exit(&criticalSection);
                scanner.move_toX0Y0(vector[1], vector[2], vector[3],vector[4]);
                break; 
              }     
case LID_MOVE_TOZ0:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);
                scanner.LID_move_toZ0(vector[1], vector[2], vector[3], vector[4], vector[5]);
                break; 
              }            
case SCANNING:
              {
                critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
                 DrawDone=true;
                 scanner.scan_update
                          ({
                             static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]),
                             static_cast<uint8_t>(vector[3]),  static_cast<uint8_t>(vector[4]),
                             static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
                             static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
                             static_cast<uint8_t>(vector[9]),  static_cast<uint8_t>(vector[10]),
                             static_cast<uint16_t>(vector[11]),static_cast<uint16_t>(vector[12]),
                             static_cast<uint8_t>(vector[13]), static_cast<int16_t>(vector[14]),  //add 240122            
                             static_cast<uint8_t>(vector[15]), static_cast<uint8_t>(vector[16]),
                             static_cast<uint16_t>(vector[17]),static_cast<uint16_t>(vector[18]),
                             static_cast<uint8_t>(vector[19]), static_cast<uint8_t>(vector[20]),
                             static_cast<uint16_t>(vector[21]),static_cast<uint16_t>(vector[22]),
                             static_cast<int16_t>(vector[23])
                            }
                           );  
                critical_section_exit(&criticalSection);
                if (!scanner.getHoppingFlg())  {
                                                 if (!scanner.getLinearFlg()) {scanner.start_scan(vector);   }
                                                 else                         {scanner.start_scanlin(vector);}    
                                               }
                else                           {
                                                 if (!scanner.getLinearFlg()) {scanner.start_hopingscan(vector);   }
                                                 else                         {scanner.start_hopingscanlin(vector);}
                                               }
                critical_section_enter_blocking(&criticalSection);
                 DrawDone=true;
                critical_section_exit(&criticalSection); 
                break; 
              }
case SENDDATALIN:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);
                scanner.readDATALin();
                break;
              }              
case FASTSCANNING:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);
                scanner.start_fastscan(vector);  
                break; 
              }   

case SET_PID_GAIN:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);
               if (!flgVirtual) set_GainPID((uint8_t)vector[1]);  //240320
                break; 
              }  
case SET_AMPLMOD_GAIN: // усиление раскачка зонда 
              {
                critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
                critical_section_exit(&criticalSection);
                 if (!flgVirtual)  set_GainApmlMod((uint8_t)vector[1]);
                 break;
              }   
case InitDAC_BIAS_SET_POINT:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);

                if (!flgVirtual)  init_DACSPB(vector[1]);       
                break;         
              }   
case InitDAC_Z:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);

                if (!flgVirtual)  init_DACZ(vector[1]);       
                break;         
              }   
case InitDAC_XY:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);

                init_DACXY(vector[1]);
                break; 
              } 
case SetDACZeroCmd:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);

                set_DACZero();
                break;
              } 
case SET_BIAS:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);

              //  '2'+'8'+'0'+'1'+'1'
              //  init_SPI(vector[1],vector[2],vector[3],vector[4]);//19, 2, 8, 0, 1, 1, value	
                set_Bias(vector[1]);
                break;
              }               
case SET_SETPOINT:
              {
                critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
                critical_section_exit(&criticalSection);
              // init_SPI(vector[1],vector[2],vector[3],vector[4]);//22, 2, 8, 0, 1, 0, value	
                set_SetPoint(vector[1]);
                break; 
              }   
  case SET_Z: { 
                critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
                critical_section_exit(&criticalSection);
                set_DACZ(vector[1]);
                break;
              }     
 case SET_XY: {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);
               init_SPI(vector[1],vector[2],vector[3],vector[4]);//29, 3, 8, 0, 1, 1, value	
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
case ADC_READCmd:
              {
               critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
               critical_section_exit(&criticalSection);
               if (ADC_IS_READY_TO_READ)
               {
                scanner.readADC();
               }
               break;
              }    
case GET_CURRENTX0Y0:
              {
                critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
                critical_section_exit(&criticalSection);
                scanner.getX0Y0();
                break;
              }   
case SCANNER_RETRACT_PROTRACT:
              {
                critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
                critical_section_exit(&criticalSection);
                scanner.scanner_retract_protract(vector[1],vector[2]); //int port=6 , int flg
                // vector[2] == 1 ? io_ports[vector[1]].enable() : io_ports[vector[1]].disable();
                break;
              } 
case SPECTROSOPY_IV:
              {
                critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
                critical_section_exit(&criticalSection);
                scanner.spectroscopyIV(vector);
                break;
              }  
case SPECTROSOPY_AIZ:
              {
                critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
                critical_section_exit(&criticalSection);
                scanner.spectroscopyAIZ(vector);
                break;
              }  

default:      {/*activateError();*/  break;}                                                                                                                                            
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
