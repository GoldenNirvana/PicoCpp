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
/*HARDWARE::HARDWARE()
{
  dac8563_1=new DAC8563(1); // DAC BIAS,SetPoint
  dac8563_2=new DAC8563(2); // DAC X,Y
  dac8563_3=new DAC8563(1);
      busy=new InputPort(16); // FIXME TEMP!!!
      conv=new OutputPort(7);
       dec=new OutputPort(10);
 resetPort=new OutputPort(17); 
   ledPort=new OutputPort(PICO_DEFAULT_LED_PIN);
    rdbLed=new OutputPort(23); 
     io1_0=new OutputPort(11);
     io1_1=new OutputPort(12);
     io2_0=new OutputPort(13);
     io2_1=new OutputPort(14); 
     io2_2=new OutputPort(15); 
     io3_0=new OutputPort(26); //вытянуть сканнер
     io3_1=new OutputPort(27); //втянуть сканнер
     io_ports.push_back(io1_0);
     io_ports.push_back(io1_1);
     io_ports.push_back(io2_0);
     io_ports.push_back(io2_1);
     io_ports.push_back(io2_2);
     io_ports.push_back(io3_0);
     io_ports.push_back(io3_1); 

   //  {1,2,116,7,10,17,PICO_DEFAULT_LED_PIN,23,11,12,12,14,15,26,27}
   */
 
  dac8563_1=new DAC8563(confighardware.DACBSPT); // DAC BIAS,SetPoint
  dac8563_2=new DAC8563(confighardware.DACXY); // DAC X,Y
  dac8563_3=new DAC8563(confighardware.DAC);
      busy=new InputPort(confighardware.BUSY); // FIXME TEMP!!!
      conv=new OutputPort(confighardware.CONV);
       dec=new OutputPort(confighardware.DEC);
 resetPort=new OutputPort(confighardware.ResetPort); 
   ledPort=new OutputPort(PICO_DEFAULT_LED_PIN);
    rdbLed=new OutputPort(confighardware.RDBPort); 
     io1_0=new OutputPort(confighardware.IO1_0);
     io1_1=new OutputPort(confighardware.IO1_1);
     io2_0=new OutputPort(confighardware.IO2_0);
     io2_1=new OutputPort(confighardware.IO2_1); 
     io2_2=new OutputPort(confighardware.IO2_2); 
     io3_0=new OutputPort(confighardware.IO3_0); //вытянуть сканнер
     io3_1=new OutputPort(confighardware.IO3_1); //втянуть сканнер
     io_ports.push_back(io1_0);
     io_ports.push_back(io1_1);
     io_ports.push_back(io2_0);
     io_ports.push_back(io2_1);
     io_ports.push_back(io2_2);
     io_ports.push_back(io3_0);
     io_ports.push_back(io3_1); 
}

HARDWARE::~HARDWARE()
{
 delete(dac8563_1);
 delete(dac8563_2);
 delete(dac8563_3);
}
void HARDWARE::set_io_value(int port, int value)  
{
  SET_IO_VALUE = false;
  if (port == 1)
  {
    std::string binary = std::bitset<2>(value).to_string();
    binary[1] == '1' ? io1_0->enable() : io1_0->disable();
    binary[0] == '1' ? io1_1->enable() : io1_1->disable();
  } 
  else
  if (port == 2) //gain
  {
    std::string binary = std::bitset<3>(value).to_string();
    binary[2] == '1' ? io2_0->enable() : io2_0->disable();
    binary[1] == '1' ? io2_1->enable() : io2_1->disable();
    binary[0] == '1' ? io2_2->enable() : io2_2->disable();
  }
  else if (port == 3)
  {
    std::string binary = std::bitset<2>(value).to_string();
    binary[1] == '1' ? io3_0->enable() : io3_0->disable();
    binary[0] == '1' ? io3_1->enable() : io3_1->disable();
  }
}
void HARDWARE::setDefaultSettings()
{
  /// BASIC SETTINGS
  uart_init(uart1, 115200);
 // uart_init(uart0, 256000);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  gpio_pull_down(resetPort->getPort());

//#warning should be undeleted
//  RX_core rxCore;
// fixme mb should add & before isr
  gpio_set_irq_enabled_with_callback(busy->getPort(), GPIO_IRQ_EDGE_FALL, true, RX_core::comReceiveISR);

  multicore_launch_core1(RX_core::launchOnCore1);

  dec->enable();
  conv->enable();
  resetPort->disable();
  gpio_pull_down(resetPort->getPort());
  ledPort->enable();
  dark();
  //io3_1.disable(); 
  uint16_t ti=7<<8; //240403
  set_GainPID(ti);  //установить минимальное усиление 240209
  
  retract();// 240403 ???
  io3_1->blink();  //втянуть   240209
 
  init_DACSPB(2);//инициирование ЦАП1  SetPoint,BIAS

  init_DACXY(3); //инициирование ЦАП2  DACXY

  init_DACZ(4);  //инициирование ЦАП3  DACZ
  
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

void HARDWARE::init_DACSPB(uint8_t port) //  4 для подставки
{
  dac8563_1->initialize(port); //code 23
/*
  afc.clear();
  afc = "code"+std::to_string(DEBUG)+ "debug Init DACSPB " + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
 */ 
}

void HARDWARE::init_DACXY(uint8_t port)
{
  dac8563_2->initialize(port); //code 27
  dac8563_2->setSpiProps();
  dac8563_2->writeA(0);
  dac8563_2->writeB(0);
 /* afc.clear();
  afc ="code"+std::to_string(DEBUG)+ "debug Init DACXY 0,0 port=" + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
  */
}

void HARDWARE::init_DACZ(uint8_t port)
{
  dac8563_3->initialize(port); //code 27
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
 dac8563_2->writeA(x);
}
void HARDWARE::move_scannerY(int y)
{
 dac8563_2->writeB(y);

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
     dac8563_1->writeB(Bias+ShiftDac);
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
     dac8563_1->writeA(SetPoint+ShiftDac);
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

void HARDWARE::set_GainPID(int gain)
{
  uint8_t ti;
  uint8_t tiadd;
  ti=(uint8_t)(gain>>8);
  tiadd=(uint8_t)(gain&0x00FF);
  if (!flgVirtual) 
  {
    set_io_value(2, ti); 
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
  dac8563_2->setSpiProps();
  if (channel == 0)  dac8563_2->writeA(value);
  if (channel == 1)  dac8563_2->writeB(value);
  sleep_us(2);// 240405
}

void HARDWARE::set_DACZ(int16_t value) 
{
  dac8563_3->setSpiProps(); 
  dac8563_3->writeA(int32_t(value)+ShiftDac);
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

void HARDWARE::retract() //втянуть
{
  io3_1->enable();  //  port 6   элемент массива портов 
}
void HARDWARE::retract(int16_t HeightJump) //втянуть на HeightJump
{
 retract(); 
 set_DACZ(-abs(HeightJump)); 
}

void HARDWARE::protract() //вытянуть
{
  io3_1->disable();  //port 6
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
void HARDWARE::LOOP_freeze_unfreeze(int port, int flg) // port virtual 5
{
//flg == 1 ? io_ports[port - 1].enable() : io_ports[port - 1].disable();
 flg == 1 ? io_ports[port]->enable() : io_ports[port]->disable();
}
void HARDWARE::freezeLOOP(uint16_t delay)    // заморозить ПИД
{
  io3_0->enable(); // 5 элемент массива портов ???
  sleep_ms(delay);
}

void HARDWARE::unfreezeLOOP(uint16_t delay)  // разморозить ПИД
{
  io3_0->disable();  // 5 элемент массива портов ???
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