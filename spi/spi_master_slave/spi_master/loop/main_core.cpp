#include "main_core.hpp"
#include <bitset>
#include <iostream>
#include "../utilities/hardcoded_functions.hpp"
#include "common_data/common_variables.hpp"
#include "../physical_devices/scanner.hpp"
#include "../utilities/debug_logger.hpp"
#include <cmath>

void  MainCore::setAlgCodeNone()
{
  if (flgСritical_section) critical_section_enter_blocking(&criticalSection);
   ALGCODE=ALGNONE;
  if (flgСritical_section) critical_section_exit(&criticalSection);    
}

void MainCore::loop()
{
 
  uint64_t time = 0;
  while (time++ < UINT64_MAX - 1000)
  {
    switch (ALGCODE)
    {
case   ALGNONE:{break;}
case RESONANCE:
              {
               setAlgCodeNone();
               scanner->start_frqscan();
               break;
              }
case APPROACH:{
                setAlgCodeNone();
                scanner->hardware->blue();
                scanner->approacphm(vector);
                scanner->hardware->green();
              //  dark();
                break;
              }
case TESTMOVER:{
                 setAlgCodeNone();
                 scanner->testpiezomover(vector);
                 break;
               }
case FREQ_SET:{
                setAlgCodeNone();
                scanner->hardware->set_Freq((uint16_t) vector[1]);              
                break;
              }
case LID_MOVE_UNTIL_STOP:
              {
                setAlgCodeNone();
                scanner->positioningXYZ(vector);               
                break;
              }
case MOVE_TOX0Y0:
              {
                setAlgCodeNone();
                scanner->move_toX0Y0(vector[1], vector[2], vector[3],vector[4]);
                break; 
              }     
case LID_MOVE_TOZ0:
              {
                setAlgCodeNone();
                scanner->LID_move_toZ0(vector[1], vector[2], vector[3], vector[4], vector[5]);
               break; 
              }            
case SCANNING:
              {
                if (flgСritical_section) critical_section_enter_blocking(&criticalSection);
                 ALGCODE=ALGNONE;
                 DrawDone=true;
                 scanner->scan_update
                          ({
                             static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]),
                             static_cast<uint8_t>(vector[3]),  static_cast<uint8_t>(vector[4]),
                             static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
                             static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
                             static_cast<uint8_t>(vector[9]),  static_cast<uint16_t>(vector[10]),
                             static_cast<uint16_t>(vector[11]),static_cast<uint16_t>(vector[12]),
                             static_cast<uint8_t>(vector[13]), static_cast<int16_t>(vector[14]),  
                             static_cast<uint8_t>(vector[15]), static_cast<uint8_t>(vector[16]),
                             static_cast<uint16_t>(vector[17]),static_cast<uint16_t>(vector[18]),
                             static_cast<uint8_t>(vector[19]), static_cast<uint8_t>(vector[20]),
                             static_cast<uint16_t>(vector[21]),static_cast<uint16_t>(vector[22]),
                             static_cast<int16_t>(vector[23])
                            }
                           );  


                if (flgСritical_section) critical_section_exit(&criticalSection);
                if (!scanner->getHoppingFlg()){
                                              if (!scanner->getLinearFlg()) {scanner->start_scan(vector);   }
                                              else                         {scanner->start_scanlin(vector);}    
                                             }
                else                         {
                                              if (!scanner->getLinearFlg()) {scanner->start_hopingscan(vector);   }
                                              else                         {scanner->start_hopingscanlin(vector);}
                                             }
                if (flgСritical_section) critical_section_enter_blocking(&criticalSection);
                 DrawDone=true;
                if (flgСritical_section)critical_section_exit(&criticalSection); 
                break; 
              }
case SENDDATALIN:
              {
                setAlgCodeNone();
                scanner->readDATALin();
                break;
              }              
case FASTSCANNING:
              {
                setAlgCodeNone();
                scanner->start_fastscan(vector);  
                break; 
              }   

case SET_PID_GAIN:
              {
                setAlgCodeNone();
                scanner->hardware->set_GainPID((uint16_t)vector[1]);  
                break; 
              }  
case SET_AMPLMOD_GAIN: // усиление раскачка зонда 
              {
                setAlgCodeNone();
                scanner->hardware->set_GainApmlMod((uint8_t)vector[1]);
                break;
              }   
case InitDAC_BIAS_SET_POINT:
              {
                setAlgCodeNone();
                if (!flgVirtual)   scanner->hardware->init_DACSetPointBias(vector[1]);       
                break;         
              }   
case InitDAC_Z:
              {
                setAlgCodeNone();
                if (!flgVirtual)   scanner->hardware->init_DACZ(vector[1]);       
                break;         
              }   
case InitDAC_XY:
              {
                setAlgCodeNone();
                scanner->hardware->init_DACXY(vector[1]);
                break; 
              } 
case SetDACZeroCmd:
              {
                setAlgCodeNone();
                scanner->hardware->set_DACZero();
                break;
              } 
case SET_BIAS:
              {
                setAlgCodeNone();
                scanner->hardware->set_Bias(vector[1]);
                break;
              }               
case SET_SETPOINT:
              {
                setAlgCodeNone();
                scanner->hardware->set_SetPoint(vector[1]);
                break; 
              }   
  case SET_Z: { 
                setAlgCodeNone();
                scanner->hardware->set_DACZ(vector[1]);
                break;
              }     
 case SET_XY: {
                setAlgCodeNone();
                scanner->hardware->init_SPI(vector[1],vector[2],vector[3],vector[4]);//29, 3, 8, 0, 1, 1, value	
                if (vector[5] == 0)
                {
                 scanner->hardware->move_scannerX(vector[6]);
                } 
                else 
                if (vector[5] == 1)
                {
                 scanner->hardware->move_scannerY(vector[6]);
                }
                break;
              }   
case ADC_READCmd: //TIMER
              {
                setAlgCodeNone();
                if (ADC_IS_READY_TO_READ)
                {
                 scanner->readADC();
                }
                break;
              }    
case GET_CURRENTX0Y0:
              {
                setAlgCodeNone();
                scanner->getX0Y0();
                break;
              }   
case ProtractAlgCode:
              {
                setAlgCodeNone();
                scanner->hardware->protract(); 
                break;
              } 
case  RetractAlgCode:
              {
                setAlgCodeNone();
                scanner->hardware->retract();
                break;
              }
case SPECTROSOPY_IV:
              {
                setAlgCodeNone();
                scanner->spectroscopyIV(vector);
                break;
              }  
case SPECTROSOPY_AIZ:
              {
                setAlgCodeNone();
                scanner->spectroscopyAIZ(vector);
                break;
              }  
case VersionCmd:
              {
                setAlgCodeNone();
                scanner->hardware->GetSOFTHARDWAREVersion();
                break;
              }               
default:      {/*activateError();*/  break;}                                                                                                                                            
   }
  }

/*
   
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
     */
}

MainCore::MainCore()
{

}
