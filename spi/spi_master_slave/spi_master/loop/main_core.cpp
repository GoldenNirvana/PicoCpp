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
      static int16_t approach_data[11];
      approach_data[0] = vector[1]; //set point
      approach_data[1] = vector[2]; // max
      approach_data[2] = vector[3]; // min
      approach_data[3] = vector[4]; // steps
      approach_data[4] = vector[5]; // initdelay
      approach_data[5] = vector[6]; // gain
      approach_data[6] = vector[7]; // scannerDelay
      approach_data[7] = vector[8]; // freq
      approach_data[8] = vector[9]; // scv
      approach_data[9] = vector[10]; //  0= SFM, 1=STM ;SICMAC-2; SICMDC-3;  device type
      approach_data[10]= vector[11]; // Voltage


      scanner.approacphm(approach_data);
      //  sleep_ms(100);
      green();
      activateDark();
      continue;
    }
    if (LID_UNTIL_STOP)  // пьезо мувер позиционирование
    {
      static int16_t pos_data[9];
      pos_data[0] = vector[1]; //  int lid_name
      pos_data[1] = vector[2]; //  int f
      pos_data[2] = vector[3]; //  int p
      pos_data[3] =abs(vector[4]); //  int n
      pos_data[4] = vector[5]; //  int dir
      pos_data[5] = vector[6]; //  int Z gate max
      pos_data[6] = vector[7]; //  int Z gate min
    //  pos_data[7] = vector[8]; //  0= SFM, 1=STM ;SICMAC-2; SICMDC-3;  device type
    //  pos_data[8] = vector[9]; //  Voltage
    
      scanner.positioningXYZ(pos_data); 

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
    if (SCANNING) //scanning
    {   // int32_t vector[16];  !!!!
      scanner.update({
                         static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]),
                         static_cast<uint8_t>(vector[3]),  static_cast<uint8_t>(vector[4]),
                         static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
                         static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
                         static_cast<uint8_t>(vector[9]),  static_cast<uint8_t>(vector[10]),
                         static_cast<uint16_t>(vector[11]),static_cast<uint16_t>(vector[12]),
                         static_cast<uint8_t>(vector[13]), static_cast<uint8_t>(vector[14]),
                         static_cast<uint16_t>(vector[15])
                     });
/*
    params[0]:=ScanParams.NX;        // кол точек в линии                                                               1
    params[1]:=ScanParams.NY;        // кол линий в скане                                                               2
    params[2]:=ScanParams.ScanPath; {X+:0,Y+:1,X+-:2; Y+-=3;} //тип пути сканирования +- сьем данных туда и обратно     3
    params[3]:=ScanParams.ScanMethod;  // {0,1,2,3,4,5,6,7,8,9,10,11}                                                   4      
    params[4]:=ScanParams.MicrostepDelay;   // задержка в каждой дискрете  при движении вперед                          5
    params[5]:=ScanParams.MicrostepDelayBW; // задержка в каждой дискрете  при движении назад                           6
    params[6]:=ScanParams.XMicrostepNmb;  //step x  в дискретах                                                         7
    params[7]:=ScanParams.YMicrostepNmb;  //step y =step x пока в дискретах                                             8
    params[8]:=ScanParams.size;         // =1  Z;  =2  Z + add signal                                                   9
    params[9]:=PidParams.Ti;   // add gain!!!!!!!!!!!!!!!!!!!                                                          10 
    params[10]:=ScanParams.DiscrNumInMicroStep;  //                                                                    11  
    params[11]:=round(ScanParams.TimMeasurePoint*1000);  // mcs задержка до начала измерения сигналов                  12
    params[12]:=datatype(ScanParams.flgOneFrame);      //fast scanning                                                 13
    params[13]:=datatype(ScanParams.flgHoping);        //hope                                                          14  
    params[14]:=ScanParams.HopingDelay;                                                                                15    
*/
/*
    ScanMethod
  Const  Topography=0; // 
//долнительные сигналы
  Const  WorkF=1;     // не используется
  Const  BackPass=2;  // не используется путь X+-, Y+-
  Const  Phase=3;     //Фаза
  Const  UAM=4;       //Force Image -  Амплитуда
  Const  Spectr=5;    //пока не надо
  Const  Litho=6;     //пока не надо
  Const  CurrentScan=7; //ток
  Const  FastScan=8;    //быстрое сканирование ток ?
  Const  TopoError=9;
  Const  FastScanPhase=10;  // //пока не надо - быстрое сканирование ток
  Const  OneLineScan=11;    //сканированиепо одной линии
*/
      if (!scanner.getHoppingFlg())  { scanner.start_scan();       }
      else                           { scanner.start_hopingscan(); }
      continue;
    }
    if (FASTSCANNING)
    {// int32_t vector[16];
      scanner.update({
                         static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]),
                         static_cast<uint8_t>(vector[3]),  static_cast<uint8_t>(vector[4]),
                         static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
                         static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
                         static_cast<uint8_t>(vector[9]),  static_cast<uint8_t>(vector[10]),
                         static_cast<uint16_t>(vector[11]),static_cast<uint16_t>(vector[12]),
                         static_cast<uint8_t>(vector[13])
                     }); 
      scanner.start_fastscan();
      FASTSCANNING = false;
      continue;
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
    if (SET_AMPLMOD_GAIN) //(AD8400_SET_GAIN) // усиление раскачка зонда
    {
    //  AD8400_SET_GAIN = false;
      SET_AMPLMOD_GAIN=false;
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
/* 
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
*/
 if ( SETBIAS) 
    {
      SETBIAS=false;
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
 
 /*if (DAC8563_SET_VOLTAGE_1)
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
*/
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
    //  if (flgDebugLevel <= DEBUG_LEVEL)       logger("ReadADC\n");
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
            switch (vector[1])
         {
          case 0: 
                {
                 SignalValue = (int16_t) ptr[AmplPin];
                 break;  
                } 
          case 1:
          case 3:  
                {
                 SignalValue = (int16_t) ptr[IPin];
                 break;  
                } 
          }       
    
        //  set_io_value(2, vector[1]);   //add 231114 gain pid
        //  set_gainPID(vector[1]);       //add 231114 gain pid
          afc +=',' + std::to_string(ZValue) + ',' + std::to_string(SignalValue) + ',' + std::to_string(vector[1]) + "\n";
          std::cout << afc;
          afc.clear();
        } 
        else
        {
          afc += ',' + std::to_string(ZValue) + ',' + std::to_string(SignalValue) +',' + std::to_string(vector[1])+"\n";   //Z,Signal
          std::cout << afc;
          afc.clear();
          sleep_ms(100);
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
      if (!flgVirtual) set_gainPID(vector[1]);//set_io_value(2, vector[1]); //231213
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
