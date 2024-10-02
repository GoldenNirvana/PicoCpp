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
   ALGCODE=ALGNONE;
}

void MainCore::launchOnCore1()
{ 
  while (true)
  {
    parse(vector,vupdateparams); //wait for data ! парсинг входящих данных из ПК 
    if (vector.size()!=0)
    {
     switch (vector[0])
     {
     /* case  ADC_RESET: 
        ADC_RESET = true;
        break;
     */   
      case VirtualCmd : //флаг симуляции работы микроконтроллера      
    //    flgVirtual=(bool)vector[1];
          flgVirtual=(bool)vector[1];
          afc.clear();
          afc = code+std::to_string(DEBUG)+"set virtual "+ std::to_string(vector[1]);
          afc +=endln;//"\n";
          std::cout << afc;
          afc.clear();
          sleep_ms(100);               
        break;
      case DebugLevelCmd: // флаг вывода отладочной информации debug level =2;  =3 запрет вывода!
        flgDebugLevel=vector[1];
        break;    
      case DebugCmd: // флаг вывода отладочной информации  =1, нет =0
        flgDebug=(bool)vector[1];
        afc.clear();
        afc = code+std::to_string(DEBUG)+"debug Set Debug "+ std::to_string(flgDebug);
        afc += endln;//"\n";
        std::cout << afc;
        afc.clear();
        sleep_ms(100); 
        break;
      case SetUseCritialSectAlgCode: // флаг использовать Сritical_section
        flgСritical_section=(bool)vector[1]; 
        break;
  //***************************************     
      case ADC_GET_VALUECmd:            
        ADC_GET_VALUE = true;// прочитатать сигналы АЦП      
        break;
      case TheadDoneCmd:
        TheadDone = true;
        break;
      case DRAWDONECmd: 
        DrawDone = true;
        break;  
      case STOPCmd:
        STOP=true; // stop the active algorithm 
        break; 
      default: 
      {
         if (vector[0]>=0 && vector[0]<100)  {ALGCODE=(int16_t)vector[0]; }
                                       else ALGCODE=0;
        break;
      }  
     }   
    continue;
   } 
   if (vupdateparams.size()!=0)  
   {
    if (vupdateparams[0]==CONFIG_UPDATECmd)
    {
       CONFIG_UPDATE = true;
    } 
   }
  }//while
}

void MainCore::loop()
{
 
  uint64_t time = 0;
  while (time++ < UINT64_MAX - 1000)
  {
    switch (ALGCODE)
    {
case   ALGNONE:{break;}
case VersionCmd:
              {
                ALGCODE=ALGNONE;
                afc.clear();
                afc = code+std::to_string(DEBUG)+" get version ";
                afc +=endln;//"\n";
                std::cout << afc;
                afc.clear();
                sleep_ms(100);
                scanner->hardware->GetSOFTHARDWAREVersion();
                break;
              } 
/*
 case  VirtualCmd:
              {
                ALGCODE=ALGNONE;
                flgVirtual=(bool)vector[1];
                afc.clear();
                afc = code+std::to_string(DEBUG)+" virtual "+ std::to_string(vector[1]);
                afc +=endln;//"\n";
                std::cout << afc;
                afc.clear();
              //  sleep_ms(100);               
                break;
              }
*/              
case ChangeHardWare:
              {
                ALGCODE=ALGNONE;
                afc.clear();
                afc =code+std::to_string(ChangeHardWare)+ " hardware "+ std::to_string(vector[1]);
                afc += endln;//"\n";
                std::cout << afc;
                afc.clear();
                sleep_ms(300);       
                if (HARDWAREVERSION!= (int8_t)vector[1])                                  
                { 
                  HARDWAREVERSION= (int8_t)vector[1];   
                  switch (HARDWAREVERSION)
                  {       
                    case 0:{
                             scanner=new  Scanner(confighardwarev0); 
                             scanner->hardware->setDefaultSettings(confighardwarev0);                 
                             break; 
                           }
                    case 1:{
                             scanner=new  Scanner(confighardwarev1);
                             scanner->hardware->setDefaultSettings(confighardwarev1);
                             break;
                           }
                  } 
                  if (scanner==nullptr) 
                  {
                   afc.clear();
                   afc =code+std::to_string(ChangeHardWare)+ "error new scanner create "+std::to_string(HARDWAREVERSION);
                   std::cout << afc;
                   afc.clear();
                   sleep_ms(100); 
                  }
                }
                else
                {
                 /* if (scanner!=nullptr) delete(scanner);
                  HARDWAREVERSION_I= (uint8_t)vector[1];   
                  switch (HARDWAREVERSION_I)
                  {       
                    case 0:   { scanner=new  Scanner(confighardwarev0); break; }
                    case 1:   { scanner=new  Scanner(confighardwarev1); break; }
                  } 
                  scanner->hardware->setDefaultSettings();
                  */
                }
               break;
              } 
case ADC_RESET:
              {
             
                
              //  scanner->hardware->ADC_reset();
                break;
              }               
case RESONANCE:
              {
               ALGCODE=ALGNONE;
               scanner->start_frqscan();
               break;
              }
case APPROACH:{
                ALGCODE=ALGNONE;
                scanner->hardware->blue();
                scanner->approacphm(vector);
                scanner->hardware->green();
              //  dark();
                break;
              }
case TESTMOVER:{
                 ALGCODE=ALGNONE;
                 scanner->testpiezomover(vector);
                 break;
               }
case FREQ_SET:{
                ALGCODE=ALGNONE;
                scanner->hardware->set_Freq((uint16_t) vector[1]);              
                break;
              }
case LID_MOVE_UNTIL_STOP:
              {
                ALGCODE=ALGNONE;
                scanner->positioningXYZ(vector);               
                break;
              }
case MOVE_TOX0Y0:
              {
                ALGCODE=ALGNONE;
                scanner->move_toX0Y0(vector[1], vector[2], vector[3],vector[4]);
                break; 
              }     
case LID_MOVE_TOZ0:
              {
                ALGCODE=ALGNONE;
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
                                              else                          {scanner->start_scanlin(vector);}    
                                             }
                else                         {
                                              if (!scanner->getLinearFlg()) {scanner->start_hopingscan(vector);   }
                                              else                          {scanner->start_hopingscanlin(vector);}
                                             }
                if (flgСritical_section) critical_section_enter_blocking(&criticalSection);
                 DrawDone=true;
                if (flgСritical_section)critical_section_exit(&criticalSection); 
                break; 
              }
case FASTSCANNING:
              {
                ALGCODE=ALGNONE;
                scanner->start_fastscan(vector);  
                break; 
              }  
case SPECTROSOPY_IV:
              {
                ALGCODE=ALGNONE;
                scanner->spectroscopyIV(vector);
                break;
              }  
case SPECTROSOPY_AIZ:
              {
                ALGCODE=ALGNONE;
                scanner->spectroscopyAIZ(vector);
                break;
              }                
case SENDDATALIN:
              {
                ALGCODE=ALGNONE;
                scanner->readDATALin();
                break;
              }              
case SET_PID_GAIN:
              {
                ALGCODE=ALGNONE;
                if (HARDWAREVERSION!=BBFPGA) scanner->hardware->set_GainPID((uint16_t)vector[1]);
                else                         scanner->hardware->set_GainPID((uint32_t)vector[1]); 
                break; 
              }  
case SET_PID_GAINFPGA:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->set_GainPIDFPGA((uint32_t)vector[1]); 
                break; 
              }   
case SET_AMPLMOD_GAIN: // усиление раскачка зонда 
              {
                ALGCODE=ALGNONE;
                scanner->hardware->set_GainApmlMod((uint8_t)vector[1]);
                break;
              }  
/*
case INITCOMMMUTATION:
              {
               ALGCODE=ALGNONE;
               scanner->hardware->init_commutation((uint8_t)vector[1],
                         (uint8_t)vector[2],(uint8_t)vector[3],(uint8_t)vector[4],(uint8_t)vector[5]);
                break;
              }  
              
case InitDAC_SET_POINT:
              {
                ALGCODE=ALGNONE;
                if (!flgVirtual) 
                {
                   scanner->hardware->init_DACSetPoint(vector[1]);            
                }
                break;         
              }
case InitDAC_BIAS:
              {
                ALGCODE=ALGNONE;
                if (!flgVirtual) 
                {      
                   scanner->hardware->init_DACBiasV(vector[1]);       
                }
                break;         
              }                 
case InitDAC_Z:
              {
                ALGCODE=ALGNONE;
                if (!flgVirtual)   scanner->hardware->init_DACZ(vector[1]);       
                break;         
              }   
case InitDAC_XY:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->init_DACXY(vector[1]);
                break; 
              } 
*/              
case SetDACZeroCmd:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->set_DACZero();
                break;
              } 
case SET_BIAS:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->set_BiasV(vector[1]);
                break;
              }               
case SET_SETPOINT:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->set_SetPoint(vector[1]);
                break; 
              }   
  case SET_Z: { 
                ALGCODE=ALGNONE;
                scanner->hardware->set_DACZ(vector[1]);
                break;
              }     
 case SET_XY: {
                ALGCODE=ALGNONE;
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
                ALGCODE=ALGNONE;
                if (ADC_IS_READY_TO_READ)
                {
                 scanner->readADC();
                }
                break;
              }    
case GET_CURRENTX0Y0:
              {
                ALGCODE=ALGNONE;
                scanner->getX0Y0();
                break;
              }   
case ProtractAlgCode:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->protract(); 
                break;
              } 
case RetractAlgCode:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->retract();
                break;
              }
case FreezePIDAlgCode:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->freezeLOOP(vector[1]);
                break;
              }
case UnFreezePIDAlgCode:
              {
                ALGCODE=ALGNONE;
                scanner->hardware->unfreezeLOOP(vector[1]);
                break;
              }                           
default:      {/*activateError();*/  break;}                                                                                                                                            
   }
  }
}

MainCore::MainCore()
{
   multicore_launch_core1(launchOnCore1);
   switch (HARDWAREVERSION)
   {       
    case BB:
           {
            scanner=new  Scanner(confighardwarev0); 
            scanner->hardware->setDefaultSettings(confighardwarev0);                 
            break; 
           }
    case WB:
          {
            scanner=new  Scanner(confighardwarev1);
            scanner->hardware->setDefaultSettings(confighardwarev1);
            break; 

          }
    case BBFPGA:
          { //BB+ WBFPGA
            scanner=new  Scanner(confighardwarev0); 
            scanner->hardware->setDefaultSettings(confighardwarev0);                 
            break; 
          }
   } 
   if (scanner==nullptr) 
   {
     afc.clear();
     afc =code+std::to_string(ChangeHardWare)+ "error new scanner create "+std::to_string(HARDWAREVERSION);
     std::cout << afc;
     afc.clear();
     sleep_ms(100); 
   }
}

void MainCore::parse(std::vector<int32_t> &vec)
{
  std::string s;
  getline(std::cin, s);
  Parser parser(s.data(),separator);//','); //240715
  vectorSize = parser.parseInts(vec);
}
void MainCore::parse(std::vector<int32_t> &vec,std::vector<int32_t> &vparams)
{
  std::string s;
  getline(std::cin, s);
  Parser parser(s.data(),separator); //','); 240715
  vectorSize = parser.parseInts(vec,vparams);
}
