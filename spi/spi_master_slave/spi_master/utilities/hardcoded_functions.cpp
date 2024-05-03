#include "hardcoded_functions.hpp"
#include <pico/multicore.h>
#include <iostream>
#include "../loop/common_data/common_variables.hpp"
#include "../transceiver/rx_core.hpp"
#include "../utilities/debug_logger.hpp"
#include "peripheral_functions.hpp"

#include <pico/multicore.h>
#include <bitset>

#define UART_TX_PIN 8
#define UART_RX_PIN 9

HARDWARE::HARDWARE(ConfigHardWare confighardware) 
{
      _confighardware=confighardware;
      dacbspt=new DAC8563(_confighardware.DACBiasSetPointMode); //set mode DAC BIAS,SetPoint
        dacxy=new DAC8563(_confighardware.DACXYMode);   //set mode DAC X,Y
         dacz=new DAC8563(_confighardware.DACZMode);    //set mode DACZ
     busyport=new InputPort(_confighardware.BUSYPort);  // FIXME TEMP!!!
         conv=new OutputPort(_confighardware.CONV);
          dec=new OutputPort(_confighardware.DEC);
    resetport=new OutputPort(_confighardware.ResetPort); 
      ledPort=new OutputPort(PICO_DEFAULT_LED_PIN);
       rdbLed=new OutputPort(_confighardware.RDBPort); 
        io1_0=new OutputPort(_confighardware.IO1_0);
        io1_1=new OutputPort(_confighardware.IO1_1);
     gainPID0=new OutputPort(_confighardware.GainPID0);
     gainPID1=new OutputPort(_confighardware.GainPID1); 
     gainPID2=new OutputPort(_confighardware.GainPID2); 
   freezeport=new OutputPort(_confighardware.FreezePort);//заморозить/разморозить ПИД 
 protractport=new OutputPort(_confighardware.ProtractPort);//вытянуть сканнер /втянуть сканнер  
  /*
     io_ports.push_back(io1_0); //0
     io_ports.push_back(io1_1);
     io_ports.push_back(gainPID0);
     io_ports.push_back(gainPID0);
     io_ports.push_back(gainPID0);
     io_ports.push_back(freezeport);
     io_ports.push_back(protractport); //6
  */   
}

HARDWARE::~HARDWARE()
{
 delete(dacbspt);
 delete(dacxy);
 delete(dacz);
 delete(busyport);
 delete(conv);
 delete(dec);
 delete(resetport);
 delete(ledPort);
 delete(rdbLed);
 delete(io1_0);
 delete(io1_1);
 delete(gainPID0);
 delete(gainPID1);
 delete(gainPID2);
 delete(freezeport);
 delete(protractport);
// io_ports.clear();
}
/*
void HARDWARE::set_io_value(int port, int value)  
{
  SET_IO_VALUE = false;
  if (port == 1) //????
  {
    std::string binary = std::bitset<2>(value).to_string();
    binary[1] == '1' ? io1_0->enable() : io1_0->disable();
    binary[0] == '1' ? io1_1->enable() : io1_1->disable();
  } 
  else
  if (port == 2) //gain PID
  {
    std::string binary = std::bitset<3>(value).to_string();
    binary[2] == '1' ? gainPID0->enable() : gainPID0->disable();
    binary[1] == '1' ? gainPID1->enable() : gainPID1->disable();
    binary[0] == '1' ? gainPID2->enable() : gainPID2->disable();
  }
  else if (port == 3) //0 заморозить сканнер=1; разморозить =0
  {
    std::string binary = std::bitset<2>(value).to_string();
    binary[1] == '1' ?   freezeport->enable() :   freezeport->disable();
    binary[0] == '1' ? protractport->enable() : protractport->disable();
  }
}
*/
void HARDWARE::setDefaultSettings()
{
  /// BASIC SETTINGS
  uart_init(uart1, 115200);
 // uart_init(uart0, 256000);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  gpio_pull_down(resetport->getPort());

//#warning should be undeleted
//  RX_core rxCore;
// fixme mb should add & before isr
  gpio_set_irq_enabled_with_callback(busyport->getPort(), GPIO_IRQ_EDGE_FALL, true, RX_core::comReceiveISR);

  multicore_launch_core1(RX_core::launchOnCore1);

  dec->enable();
  conv->enable();
  resetport->disable();
  gpio_pull_down(resetport->getPort());
  ledPort->enable();
  dark();

  uint16_t ti=7<<8; //240403
  set_GainPID(ti);  //установить минимальное усиление 240209
  
  retract();        //втянуть 240403 ???
//************************************************************* 
//  init_DACSetPointBias(2);   //инициирование ЦАП1  SetPoint,BIAS
// init_DACXY(3);    //инициирование ЦАП2  DACXY
//  init_DACZ(4);    //инициирование ЦАП3  DACZ

//240503 edited
  init_DACSetPointBias(_confighardware.DACBiasSetPointPort);   //инициирование ЦАП1  SetPoint,BIAS

  init_DACXY(_confighardware.DACXYPort);    //инициирование ЦАП2  DACXY
 
  init_DACZ(_confighardware.DACZPort);      //инициирование ЦАП3  DACZ
  
}

void HARDWARE::GetSOFTHARDWAREVersion()
{
 //time_t now = time(0);
// char* version = ctime(&now);
  afc.clear();
 // std::string date;
 // date=version;
  afc = "code"+std::to_string(VersionCmd)+" version soft "+ SOFTVERSION+", hardware "+HARDWAREVERSION;
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);

}

void HARDWARE::set_Freq(uint32_t freq)
{
  int64_t flag_freq = 1 << 14;
  int64_t scale = 1 << 28;
  int64_t n_reg = int64_t(freq * scale / 25.0e6);
  int64_t n_low = n_reg & 0x3fff;
  int64_t n_hi  = (n_reg >> 14) & 0x3fff;

  uint8_t buf[6];

  buf[0] = (flag_freq | n_low) / (0x100);
  buf[1] = (flag_freq | n_low) % (0x100);
  buf[2] = (flag_freq | n_hi) / (0x100);
  buf[3] = (flag_freq | n_hi) % (0x100);
  buf[4] = (0x2000) / (0x100);
  buf[5] = (0x2000) % (0x100);

  //logger(buf, 6);

  decoder.activePort(1);
  sleep_us(1); // 240411 add
  Spi::setProperties(8, 1, 1);
  spi_write_blocking(spi_default, buf, 2);
  sleep_us(1); // 240411 add
  spi_write_blocking(spi_default, buf + 2, 2);
  sleep_us(1); // 240411 add
  spi_write_blocking(spi_default, buf + 4, 2);
  sleep_us(1);
  decoder.activePort(7); //240411  add
}

void HARDWARE::get_result_from_adc()
{
  ADC_IS_READY_TO_READ = false;
  conv->disable();
  sleep_us(10);
  conv->enable();
}

void HARDWARE::init_SPI( uint8_t port ,uint8_t v2 ,uint8_t v3, uint8_t v4 )
{
 decoder.activePort(port);
 Spi::setProperties(v2, v3, v4);
}

void HARDWARE::init_DACSetPointBias(uint8_t spiport) //  4 для подставки
{
  dacbspt->initialize(spiport); //code 23
/*
  afc.clear();
  afc = "code"+std::to_string(DEBUG)+ "debug Init DACSPB " + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
 */ 
}

void HARDWARE::init_DACXY(uint8_t spiport) //spi port
{
  dacxy->initialize(spiport); //code 27
  dacxy->setSpiProps();
  dacxy->writeA(0);
  dacxy->writeB(0);
 /* afc.clear();
  afc ="code"+std::to_string(DEBUG)+ "debug Init DACXY 0,0 port=" + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
  */
}

void HARDWARE::init_DACZ(uint8_t spiport)
{
  dacz->initialize(spiport); //code 27
  set_DACZ(0); 
 /*
  afc.clear();
  afc ="code"+std::to_string(DEBUG)+ "debug Init DACZ 0 port=" + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
 */ 
}
void HARDWARE::move_scannerX(int x)
{
 dacxy->writeA(x);
}
void HARDWARE::move_scannerY(int y)
{
 dacxy->writeB(y);

}
/*
void set_Bias(int8_t channel,int32_t Bias)
{
//   code  22 , 2, 8, 0, 1, 1, value 
  if (!flgVirtual)
  {  if (channel == 0)
      {
        dac8563_1.writeA(Bias+ShiftDac);
      }
      else 
      if (channel == 1)
      {
        dac8563_1.writeB(Bias+ShiftDac);
      }	
  }
}
*/
 
 void HARDWARE::set_Bias(int32_t Bias)
{
//   code  22 , 2, 8, 0, 1, 1, value 
  if (!flgVirtual)
  { 
     dacbspt->writeB(Bias+ShiftDac);
  }	
  /*
 if  (flgDebug)
 {
  afc.clear();
  afc ="code"+std::to_string(DEBUG)+ "debug Bias"+ std::to_string(Bias);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
 }
 */
}   
/*

void set_SetPoint(int8_t channel, int32_t SetPoint)
{//  code  22, 2, 8, 0, 1, 0, value
  if (!flgVirtual)
  {
    if (channel == 0)
   {
     dac8563_1.writeA(SetPoint+ShiftDac);
   }
   else 
   if (channel == 1)
   {
     dac8563_1.writeB(SetPoint+ShiftDac);
   }	
  } 
  // отладка
  afc.clear();
  afc = "debug SetPoint " + std::to_string(channel) + ',' + std::to_string(SetPoint);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100); 
}
*/

void HARDWARE::set_SetPoint( int32_t SetPoint)
{//  code  22, 2, 8, 0, 1, 0, value
  if (!flgVirtual)
  {
     dacbspt->writeA(SetPoint+ShiftDac); // 240425 ?
  } 
  // отладка
  if  (flgDebug)
  {
   afc.clear();
   afc ="code"+std::to_string(DEBUG)+ "debug SetPoint "+ std::to_string(SetPoint);
   afc += +"\n";
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  }
}

void HARDWARE::set_GainApmlMod(uint8_t gain)
{
  uint8_t intBuf[1]; 
  if (!flgVirtual)
  { 
    decoder.activePort(5);
    Spi::setProperties(8, 0, 0);
    intBuf[0] = 0;
    spi_write_blocking(spi_default, intBuf, 1); 
  //  sleep_us(2);//240405 
    intBuf[0] = (uint8_t)gain;
    spi_write_blocking(spi_default, intBuf, 1);
 //   sleep_us(2);//240405 
    decoder.activePort(7);
  } 
     // отладка
  if (flgDebug)  
  {
   afc.clear();
   afc ="code"+std::to_string(DEBUG)+ "debug Ampl Gain "+ std::to_string(gain);
   afc += +"\n";
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  } 
}

void HARDWARE::set_GainPID(uint16_t gain)
{
  uint8_t ti;
  uint8_t tiadd;
  ti=(uint8_t)(gain>>8);
  tiadd=(uint8_t)(gain&0x00FF);
  if (!flgVirtual) 
  {
  //  set_io_value(2, ti); //???????  //240503
   /* std::string binary = std::bitset<3>(ti).to_string();
    binary[2] == '1' ? gainPID0->enable() : gainPID0->disable();
    binary[1] == '1' ? gainPID1->enable() : gainPID1->disable();
    binary[0] == '1' ? gainPID2->enable() : gainPID2->disable();
   */ 
    (ti&0x04) == 1 ? gainPID0->enable() : gainPID0->disable();
    (ti&0x02) == 1 ? gainPID1->enable() : gainPID1->disable();
    (ti&0x01) == 1 ? gainPID2->enable() : gainPID2->disable();
    // отладка
    uint8_t intBuf[1]; 
    decoder.activePort(6);
    Spi::setProperties(8, 0, 0);
    intBuf[0] = 0;
    spi_write_blocking(spi_default, intBuf, 1); 
    intBuf[0] = tiadd;
    spi_write_blocking(spi_default, intBuf, 1); 
    decoder.activePort(7);
  } 
  if (flgDebug)  
  {
   afc.clear();
   afc = "code"+std::to_string(DEBUG)+"debug PID Gain "+ std::to_string(ti)+' '+ std::to_string(tiadd);
   afc += +"\n";
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  }  
}

void HARDWARE::set_clock_enable()
{
  uint8_t intBuf[1];
  Spi::setProperties(8, 1, 1);
  decoder.activePort(7);
  spi_write_blocking(spi_default, intBuf, 1);
}
void HARDWARE::set_DACZero()
{ 
 set_DACXY(0,0); 
 set_DACXY(1,0); 
 set_DACZ(0); 
 sleep_us(10); //240405
}
void HARDWARE::set_DACXY(uint8_t channel, uint16_t value) 
{
  dacxy->setSpiProps();
  if (channel == 0)  dacxy->writeA(value);
  if (channel == 1)  dacxy->writeB(value);
  sleep_us(2);// 240405
}

void HARDWARE::set_DACZ(int16_t value) 
{
  dacz->setSpiProps(); 
  dacz->writeA(int32_t(value)+ShiftDac);
  sleep_us(2);// 240405
}


uint16_t *HARDWARE::getValuesFromAdc()
{
  repeatTwoTimes();
  return repeatTwoTimes();
}

uint16_t *HARDWARE::repeatTwoTimes()
{
  get_result_from_adc();
  int j = 0;
  while (!ADC_IS_READY_TO_READ && j++ < 3)
  {
    sleep_us(100);
  }
  return spiBuf;
}
/*
void HARDWARE::scanner_retract_protract(int port, int flg) 
// port  5  1- втянуть,     0-вытянуть
// port  6  1- заморозить,  0-разморозить
 {
  afc.clear();
  if (flg == 0)
  {     
     switch (port)
   {  
   case 5: { io_ports[port]->enable();  afc = " scanner retract " + std::to_string(port); break;}
   case 6: { io_ports[port]->enable();  afc = " PID unfreeze "    + std::to_string(port); break;}
   }
  }
  else 
  {
     switch (port)
   {  
   case 5: { io_ports[port]->disable();  afc = " scanner protract " + std::to_string(port); break;}
   case 6: { io_ports[port]->disable();  afc = " PID  freeze "      + std::to_string(port); break;}
   }
  } 
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100); 
 } 
*/
void HARDWARE::retract() //втянуть
{
  protractport->enable();  //  port 6   элемент массива портов 
}
void HARDWARE::retract(int16_t HeightJump) //втянуть на HeightJump
{
 retract(); 
 set_DACZ(-abs(HeightJump)); 
}

void HARDWARE::protract() //вытянуть
{
  protractport->disable();  //port 6
}
/*
void HARDWARE::protract(uint16_t delay,int16_t DacZ0,int16_t HeightJump) //вытянуть
{
 // unfreezeLOOP(delay); 
   protract();
  //set_DACZ(0,0); 
   ZMove(DacZ0,HeightJump,-20, delay);
}
*/
/*
void HARDWARE::LOOP_freeze_unfreeze(int port, int flg) // port virtual 5
{
//flg == 1 ? io_ports[port - 1].enable() : io_ports[port - 1].disable();
 flg == 1 ? io_ports[port]->enable() : io_ports[port]->disable();
}
*/
void HARDWARE::freezeLOOP(uint16_t delay)    // заморозить ПИД
{
  freezeport->enable(); // 5 элемент массива портов ???
  sleep_ms(delay);
}

void HARDWARE::unfreezeLOOP(uint16_t delay)  // разморозить ПИД
{
  freezeport->disable();  // 5 элемент массива портов ???
  sleep_ms(delay);
}
void HARDWARE::activateError()
{
  while (true)
  {
    ledPort->enable();
    activateRed();
    sleep_ms(1000);
    ledPort->disable();
    activateDark();
    sleep_ms(1000);
  }
}

void HARDWARE::_delay_us(double __us)
{
  uint32_t __count =
    (uint32_t) (__us / 0.008) - 3; // 8ns per cycle for 125MHz, from experimentation remove 3cycles for overhead
  busy_wait_at_least_cycles(__count);
}

void HARDWARE::green()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateGreen();
    sleep_ms(100);
  }
}

void HARDWARE::blue()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateBlue();
    sleep_ms(100);
  }
}

void HARDWARE::red()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateRed();
    sleep_ms(100);
  }
}

void HARDWARE::dark()
{
  int i = 0;
  while (i++ < 3)
  {
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
    activateDark();
    sleep_ms(100);
  }
}

void HARDWARE::activateGreen()
{
  rdbLed->disable();
  sleep_us(60);
  for (int i = 0; i < 8; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(85);
    rdbLed->disable();
    busy_wait_at_least_cycles(35);
  }

  for (int i = 0; i < 16; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(35);
    rdbLed->disable();
    busy_wait_at_least_cycles(85);
  }
}

void HARDWARE::activateRed()
{
  rdbLed->disable();
  sleep_us(60);
  for (int i = 0; i < 8; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(35);
    rdbLed->disable();
    busy_wait_at_least_cycles(85);
  }

  for (int i = 0; i < 8; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(85);
    rdbLed->disable();
    busy_wait_at_least_cycles(35);
  }

  for (int i = 0; i < 8; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(35);
    rdbLed->disable();
    busy_wait_at_least_cycles(85);
  }
}

void HARDWARE::activateBlue()
{
  rdbLed->disable();
  sleep_us(60);
  for (int i = 0; i < 16; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(35);
    rdbLed->disable();
    busy_wait_at_least_cycles(85);
  }
  for (int i = 0; i < 8; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(85);
    rdbLed->disable();
    busy_wait_at_least_cycles(35);
  }
}

void HARDWARE::activateDark()
{
  rdbLed->disable();
  sleep_us(60);
  for (int i = 0; i < 24; ++i)
  {
    rdbLed->enable();
    busy_wait_at_least_cycles(35);
    rdbLed->disable();
    busy_wait_at_least_cycles(85);
  }
}