#include "hardcoded_functions.hpp"
#include <pico/multicore.h>
#include <iostream>
//#include "../loop/common_data/common_variables.hpp"
#include "../transceiver/rx_core.hpp"
#include "../utilities/debug_logger.hpp"
#include "peripheral_functions.hpp"

#include <pico/multicore.h>
#include <bitset>

HARDWARE::HARDWARE(ConfigHardWare confighardware)   // BB  mother BB+FPGA
{
   /*   _confighardware=confighardware;
      dacbspt=new DAC8563(_confighardware.DACBiasSetPointMode); //set mode DAC BIAS,SetPoint
        dacxy=new DAC8563(_confighardware.DACXYMode);   //set mode DAC X,Y
         dacz=new DAC8563(_confighardware.DACZMode);    //set mode DAC Z  
     busyport=new InputPort(_confighardware.BUSYPort);
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
 */
       dacspt=new DAC8563(confighardware.DACSetPointMode); //set mode DAC BIAS,SetPoint
        dacbv=new DAC8563(confighardware.DACBiasVMode); //set mode DAC BIAS,SetPoint
        dacxy=new DAC8563(confighardware.DACXYMode);   //set mode DAC X,Y
         dacz=new DAC8563(confighardware.DACZMode);    //set mode DAC Z  
     busyport=new InputPort(confighardware.BUSYPort);
         conv=new OutputPort(confighardware.CONV);
          dec=new OutputPort(confighardware.DEC);
    resetport=new OutputPort(confighardware.ResetPort); 
      ledPort=new OutputPort(PICO_DEFAULT_LED_PIN);
       rdbLed=new OutputPort(confighardware.RDBPort); 
        io1_0=new OutputPort(confighardware.IO1_0); //??
        io1_1=new OutputPort(confighardware.IO1_1); //?
     gainPID0=new OutputPort(confighardware.GainPID0);
     gainPID1=new OutputPort(confighardware.GainPID1); 
     gainPID2=new OutputPort(confighardware.GainPID2); 
   freezeport=new OutputPort(confighardware.FreezePort);//заморозить/разморозить ПИД 
 protractport=new OutputPort(confighardware.ProtractPort);//вытянуть сканнер /втянуть сканнер  
 linearDriver=new LinearDriverPico2040(true,configlineardrivev0);   
}
HARDWARE::HARDWARE(ConfigHardWareNew confighardware) // WB
{
  //    _confighardware=confighardware;
       dacspt=new DAC8563(confighardware.DACSetPointMode); //set mode DAC BIAS,SetPoint
        dacbv=new DAC8563(confighardware.DACBiasVMode); //set mode DAC BIAS,SetPoint
        dacxy=new DAC8563(confighardware.DACXYMode);   //set mode DAC X,Y
         dacz=new DAC8563(confighardware.DACZMode);    //set mode DAC Z  
     busyport=new InputPort(confighardware.BUSYPort);
         conv=new OutputPort(confighardware.CONV);
          dec=new OutputPort(confighardware.DEC);
    resetport=new OutputPort(confighardware.ResetPort); 
      ledPort=new OutputPort(PICO_DEFAULT_LED_PIN);
       rdbLed=new OutputPort(confighardware.RDBPort); 
      //  io1_0=new OutputPort(_confighardware.IO1_0);
     //   io1_1=new OutputPort(_confighardware.IO1_1);
     gainPID0=new OutputPort(confighardware.GainPID0);
     gainPID1=new OutputPort(confighardware.GainPID1); 
     gainPID2=new OutputPort(confighardware.GainPID2); 
   freezeport=new OutputPort(confighardware.FreezePort);  //заморозить/разморозить ПИД 
 protractport=new OutputPort(confighardware.ProtractPort);//вытянуть сканнер /втянуть сканнер 
//add new ports
     modulateuport=new OutputPort(confighardware.ModulateUPort);   // вкл=1; выкд=0 модуляцию U  
         i_stmport=new OutputPort(confighardware.SD_1Port);        // порты  настройки СД I_STM=1; 0 =др
        sensorport=new OutputPort(confighardware.SD_2Port);        // порты  настройки СД Cantilever=0; 1-Piezo
      signloopport=new OutputPort(confighardware.SignLoopPort);    // знак ПИД // 0=+ ; 1=-
 integrator_inport=new OutputPort(confighardware.Interator_InPort);// выбор вход сигнала на ПИД из1-SD; 0=ПТН(I) 
      linearDriver=new LinearDriverMotherBoard(configlineardrivev1);   
 /*
      dacbspt=new DAC8563(_confighardware.DACBiasSetPointMode); //set mode DAC BIAS,SetPoint
        dacxy=new DAC8563(_confighardware.DACXYMode);   //set mode DAC X,Y
         dacz=new DAC8563(_confighardware.DACZMode);    //set mode DAC Z  
     busyport=new InputPort(_confighardware.BUSYPort);
         conv=new OutputPort(_confighardware.CONV);
          dec=new OutputPort(_confighardware.DEC);
    resetport=new OutputPort(_confighardware.ResetPort); 
      ledPort=new OutputPort(PICO_DEFAULT_LED_PIN);
       rdbLed=new OutputPort(_confighardware.RDBPort); 
      //  io1_0=new OutputPort(_confighardware.IO1_0);
     //   io1_1=new OutputPort(_confighardware.IO1_1);
     gainPID0=new OutputPort(_confighardware.GainPID0);
     gainPID1=new OutputPort(_confighardware.GainPID1); 
     gainPID2=new OutputPort(_confighardware.GainPID2); 
   freezeport=new OutputPort(_confighardware.FreezePort);//заморозить/разморозить ПИД 
 protractport=new OutputPort(_confighardware.ProtractPort);//вытянуть сканнер /втянуть сканнер 

modulateuport=new OutputPort(_confighardware.ModulateUPort);   // вкл=1; выкд=0 модуляцию U  
    i_stmport=new OutputPort(_confighardware.SD_1Port);        // порты  настройки СД I_STM=1; 0 =др
  sensorport=new OutputPort(_confighardware.SD_2Port);        // порты  настройки СД Cantilever=0; 1-Piezo
  signloopport=new OutputPort(_confighardware.SignLoopPort);    // знак ПИД // 0=+ ; 1=-
 integrator_inport=new OutputPort(_confighardware.Interator_InPort);// выбор вход сигнала на ПИД из1-SD; 0=ПТН(I) 
 */
}

HARDWARE::~HARDWARE()
{
    delete(dacspt);
    delete(dacbv);
    delete(dacxy);
    delete(dacz);
    delete(busyport);
    delete(conv);
    delete(dec);
    delete(resetport);
    delete(ledPort);
    delete(rdbLed);
    delete(io1_0); //??
    delete(io1_1); //??
    delete(gainPID0);
    delete(gainPID1);
    delete(gainPID2);
    delete(freezeport);
    delete(protractport);
   if (HARDWAREVERSION>BB) //WB+WBFPGA
   {       
     delete(modulateuport);
     delete(i_stmport);
     delete(sensorport);
     delete(signloopport);
     delete(integrator_inport);
   }
   if (linearDriver!=0) delete(linearDriver);
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
void HARDWARE::reset_ADCPort()
{
  resetport->enable();
  sleep_us(10);
  resetport->disable();
}
void HARDWARE::setDefaultSettings(ConfigHardWare  confighardwarev)  // BB,BBFPGA
{
 // BASIC SETTINGS
 // uart_init(uart1, 115200); //????
 // uart_init(USB_UART_ID, 115200); //????
 // gpio_set_function(USBUART_TX_PIN, GPIO_FUNC_UART);
 // gpio_set_function(USBUART_RX_PIN, GPIO_FUNC_UART); 
 // gpio_pull_down(resetport->getPort());
  if (HARDWAREVERSION==BBFPGA)
  {
    ShiftDac=0;
    SetPointScale=2;
    uart_init(FPGA_UART_ID, FPGA_BAUD_RATE); //add  240627
    gpio_set_function(FPGAUART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(FPGAUART_RX_PIN, GPIO_FUNC_UART);  
    // Enable UART
    uart_set_hw_flow(FPGA_UART_ID, false, false);
    uart_set_format(FPGA_UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(FPGA_UART_ID,true);// true);  
   // uart_set_hw_flow(FPGA_UART_ID,true, true);
  }
  gpio_pull_down(resetport->getPort());
// #warning should be undeleted
// RX_core rxCore;
// fixme mb should add & before isr
  gpio_set_irq_enabled_with_callback(busyport->getPort(), GPIO_IRQ_EDGE_FALL, true, RX_core::comReceiveISR);

 // multicore_launch_core1(RX_core::launchOnCore1); // 240508 ??

  dec->enable();
  conv->enable();
  resetport->disable();
  gpio_pull_down(resetport->getPort());
  ledPort->enable();
  dark();
  /*if (HARDWAREVERSION==BB)
  {
    uint16_t ti=7<<8; set_GainPID(ti); //240403  ?????
  }
  else
  { } //установить минимальное усиление 240209 
  */
   init_DACSetPoint(confighardwarev.DACSetPointPort);   //инициирование ЦАП1  SetPoint
   init_DACBiasV(confighardwarev.DACBiasVPort);   //инициирование ЦАП1  BIAS
   init_DACXY(confighardwarev.DACXYPort);    //инициирование ЦАП2  DACXY
   uint32_t gain=7; 
   set_GainPID(gain); // not virtual; not debug!
   retract();         //втянуть    
   init_DACZ(confighardwarev0.DACZPort);      //инициирование ЦАП3  DACZ
   set_DACZ(0); 

//************************************************************* 
 // init_commutation(sensor,signloop,signal_to_loop,usenotmod_I,usemod_U);
 /*
    default afm probe ?????
    sensor=0           // probe=0;  cantilever =1
    signLoop:=1;       // 1= -1 ; 0 = +1
    signal_to_loop:=1; // sd->to loop =1 Ampl  
    usemod_U:=0;       // use mod U; not=0
    usenotmod_I:=1;       // use mod I not  =1 ; 
  */  

//  init_DACZ(dacZPort);      //инициирование ЦАП3  DACZ
  
}
void HARDWARE::setDefaultSettings(ConfigHardWareNew  confighardwarev) //WB  
{
 // BASIC SETTINGS
 // uart_init(uart1, 115200); //????
 // uart_init(USB_UART_ID, 115200); //????
 // gpio_set_function(USBUART_TX_PIN, GPIO_FUNC_UART);
 // gpio_set_function(USBUART_RX_PIN, GPIO_FUNC_UART); 
 // gpio_pull_down(resetport->getPort());

  gpio_pull_down(resetport->getPort());
// #warning should be undeleted
// RX_core rxCore;
// fixme mb should add & before isr
  gpio_set_irq_enabled_with_callback(busyport->getPort(), GPIO_IRQ_EDGE_FALL, true, RX_core::comReceiveISR);

 // multicore_launch_core1(RX_core::launchOnCore1); // 240508 ??

  dec->enable();
  conv->enable();
  resetport->disable();
  gpio_pull_down(resetport->getPort());
  ledPort->enable();
  dark();
  init_commutation(0 , 1 , 1 , 1, 0);   //default afm
    //init_commutation(1 , 1 , 1 , 0, 0);  //afm  240624
  init_DACSetPoint(confighardwarev.DACSetPointPort);   //инициирование ЦАП1  SetPoint
  init_DACBiasV(confighardwarev.DACBiasVPort);   //инициирование ЦАП1  BIAS
  init_DACXY(confighardwarev.DACXYPort);    //инициирование ЦАП2  DACXY
  uint32_t gain=7; 
  set_GainPID(gain); // not virtual; not debug!
  retract();         //втянуть    
  init_DACZ(confighardwarev0.DACZPort);      //инициирование ЦАП3  DACZ
  set_DACZ(0); 

//************************************************************* 
 // init_commutation(sensor,signloop,signal_to_loop,usenotmod_I,usemod_U);
 /*
    default afm probe ?????
    sensor=0           // probe=0;  cantilever =1
    signLoop:=1;       // 1= -1 ; 0 = +1
    signal_to_loop:=1; // sd->to loop =1 Ampl  
    usemod_U:=0;       // use mod U; not=0
    usenotmod_I:=1;       // use mod I not  =1 ; 
  */  

//  init_DACZ(dacZPort);      //инициирование ЦАП3  DACZ
  
}
void HARDWARE::GetSOFTHARDWAREVersion()
{
  afc.clear();
  afc = code+std::to_string(VersionCmd)+"soft "+ SOFTVERSION+" softhardware ver"+SoftHARDWAREVERSION
  +", hardware "+std::to_string(HARDWAREVERSION);
  afc +="\n";
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
  sleep_us(1);   // 240411 add
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
void HARDWARE::setLoopSign(int8_t value)
{
   if (HARDWAREVERSION==BBFPGA)
  {


  }
  else
  {
   switch (value)
   {
    case 0:{signloopport->disable(); break;} // +
    case 1:{signloopport->enable(); break; } // -
   }
  } 
}

void HARDWARE::setSignal_In_Loop(int8_t value)
{
    switch (value)
 {
  case 0:{integrator_inport->disable(); break;}// Ampl
  case 1:{integrator_inport->enable();  break;}// I
 }
} 
void HARDWARE::useModulateI(int8_t value)
{ 
  switch (value)
 {
  case 0:{i_stmport->disable(); break;} 
  case 1:{i_stmport->enable(); break;} //Loop use модуляцию I->SD
 }
}
 
void HARDWARE::setSensor(int8_t value)
{
   switch (value)
 {
  case 0:{sensorport->disable(); break;}// cantilever
  case 1:{sensorport->enable();  break;}// piezo
 }
} 

void HARDWARE::setModulateU(int8_t value)
{
  switch (value)
 {
  case 0:{modulateuport->disable(); break;}
  case 1:{modulateuport->enable(); break;}// вкл модуляцию U
 }
}

void HARDWARE::init_commutation(uint8_t sensor ,uint8_t loopsign ,uint8_t signal_in_loop , uint8_t usenotmod_I,uint8_t usemod_U)
{ 
 setSensor(sensor); 
 setLoopSign(loopsign);
 setSignal_In_Loop(signal_in_loop); //Ampl or I
 useModulateI(usenotmod_I);
 setModulateU(usemod_U);
}
void HARDWARE::init_SPI( uint8_t port ,uint8_t v2 ,uint8_t v3, uint8_t v4 )
{
 decoder.activePort(port);
 Spi::setProperties(v2, v3, v4);
}

void HARDWARE::init_DACSetPoint(uint8_t spiport) //  4 для подставки
{
  dacspt->initialize(spiport); //code 23
/*
  afc.clear();
  afc = code+std::to_string(DEBUG)+ "debug Init DACSPB " + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
 */
}
void HARDWARE::init_DACBiasV(uint8_t spiport) //  4 для подставки
{
  dacbv->initialize(spiport); //code 23
/*
  afc.clear();
  afc = code+std::to_string(DEBUG)+ "debug Init DACSPB " + std::to_string(port);
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
  afc =code+std::to_string(DEBUG)+ "debug Init DACXY 0,0 port=" + std::to_string(port);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
  */
}

void HARDWARE::init_DACZ(uint8_t spiport)
{
  dacz->initialize(spiport); //code 27
//  set_DACZ(0); //240921
 /*
  afc.clear();
  afc =code+std::to_string(DEBUG)+ "debug Init DACZ 0 port=" + std::to_string(port);
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
void HARDWARE::set_BiasV(int32_t BiasV)
{
//   code  22 , 2, 8, 0, 1, 1, value 
  if (!flgVirtual)
  { 
     dacbv->writeB(BiasV+ShiftDac);
  }	
   switch (HARDWAREVERSION)
      {
      case WB:
         dacbv->writeA(BiasV+ShiftDac);
        break;
      case BB:
      case BBFPGA:
         dacbv->writeB(BiasV+ShiftDac);
        break;       
      }
  /*
 if  (flgDebug)
 {
  afc.clear();
  afc =code+std::to_string(DEBUG)+ "debug Bias"+ std::to_string(Bias);
  afc += +"\n";
  std::cout << afc;
  afc.clear();
  sleep_ms(100);
 }
 */
}   
void HARDWARE::ReadDataFromFPGAArray()
{
  uint8_t szread=4;
  uint8_t szasc=40;  //array adc
  uint8_t outbuffer[szread];
  uint8_t inbuffer[szasc];
  FPGAReadDataArray readdata;
  outbuffer[0]=readdata.delimbegin;
  outbuffer[1]=readdata.cmd;
  outbuffer[2]=readdata.crcpar;
  outbuffer[3]=readdata.delimend;
  if (flgDebug)  
  {
    std::string afcc;
    afcc.clear();
    afcc=code+std::to_string(DEBUG); 
    for (size_t j = 0; j < szread; ++j)
    {
      afcc +=separator + std::to_string(outbuffer[j]);
    }
    afcc +=endln;
    std::cout << afcc;
    sleep_ms(200);
    afcc.clear();
  }
  while (!uart_is_writable(FPGA_UART_ID)){sleep_ms(30);}  
  {
    uart_write_blocking(FPGA_UART_ID, outbuffer,szread);
    sleep_ms(30);
  }
  while (!uart_is_readable(FPGA_UART_ID)){sleep_ms(30);}  
  {
    uart_read_blocking(FPGA_UART_ID, inbuffer,szasc);   
  }
  uint8_t k=0;
  for (size_t i = 0; i < 8; i++)
  {
   spiBuf[i]=(inbuffer[k+1]<<24)+(inbuffer[k+2]<<16)+(inbuffer[k+3]<<8)+inbuffer[k+4];
   k+=4;
  }
}
uint32_t HARDWARE::ReadDataFromFPGA(FPGAReadData readdata)
{
  uint8_t szread=8;
  uint8_t szasc=12;
  uint8_t outbuffer[szread];
  uint8_t inbuffer[szasc];
  outbuffer[0]=readdata.delimbegin;
  outbuffer[1]=readdata.cmd;
  outbuffer[2]=(readdata.addr&0xFF000000)>>24;
  outbuffer[3]=(readdata.addr&0x00FF0000)>>16;  
  outbuffer[4]=(readdata.addr&0x0000FF00)>>8;
  outbuffer[5]=readdata.addr&0x000000FF;
  outbuffer[6]=readdata.crcpar;
  outbuffer[7]=readdata.delimend;
  if (flgDebug)  
  {
    std::string afcc;
    afcc.clear();
    afcc=code+std::to_string(DEBUG); 
    for (size_t j = 0; j < szread; ++j)
    {
      afcc +=separator + std::to_string(outbuffer[j]);
    }
    afcc +=endln;
    std::cout << afcc;
    sleep_ms(200);
    afcc.clear();
  }
  while (!uart_is_writable(FPGA_UART_ID)){sleep_ms(30);}  
  {
    uart_write_blocking(FPGA_UART_ID, outbuffer,szread);
    sleep_ms(30);
  }
  while (!uart_is_readable(FPGA_UART_ID)){sleep_ms(30);}  
  {
    uart_read_blocking(FPGA_UART_ID, inbuffer,szasc);   
  }
  uint32_t res;
  res=(inbuffer[6]<<24)+(inbuffer[7]<<16)+(inbuffer[8]<<8)+inbuffer[9];
  return res;
}
void HARDWARE::AscResult(FPGAAscData ascdata, uint8_t* dst, size_t len)
{
  uint8_t buffer[8];
  memcpy(buffer, &ascdata, sizeof(ascdata));
  if (flgDebug)  
  {
    std::string afcc;
    afcc.clear();
    afcc=code+std::to_string(DEBUG); 
    for (size_t j = 0; j < sizeof(ascdata); ++j)
    {
      afcc +=separator + std::to_string(buffer[j]);
    }
    afcc +=endln;
    std::cout << afcc;
    sleep_ms(200);
    afcc.clear();
  }
  if (uart_is_readable(FPGA_UART_ID)) 
  {
    uart_read_blocking(FPGA_UART_ID, dst,len);   
  }
}

void HARDWARE::WriteDataToFPGA(FPGAWriteData writedata)
{
 /* big_endian !!!! старшие - младшие байты
    truct.pack("BBBBBBBBBBBB",self.DELIM, self.CMD_WRITE,\
    (address&0xFF000000)>>24,(address&0x00FF0000)>>16, (address&0x0000FF00)>>8,(address&0x000000FF),\
    (data&0xFF000000)>>24,(data&0x00FF0000)>>16, (data&0x0000FF00)>>8,(data&0x000000FF),\
    self.CMD_CRC, self.DELIM\
 */           
  size_t szwrite;
  size_t szasc=8; //for ASC FPGA
  szwrite=sizeof(writedata);
  uint8_t buffer[szwrite];
  uint8_t outbuffer[szasc];
  buffer[0]=writedata.delimbegin;
  buffer[1]=writedata.cmd;
  buffer[2]=(writedata.addr&0xFF000000)>>24;
  buffer[3]=(writedata.addr&0x00FF0000)>>16;  
  buffer[4]=(writedata.addr&0x0000FF00)>>8;
  buffer[5]= writedata.addr&0x000000FF;
  buffer[6]=(writedata.data&0xFF000000)>>24;
  buffer[7]=(writedata.data&0x00FF0000)>>16;;
  buffer[8]=(writedata.data&0x0000FF00)>>8;
  buffer[9]= writedata.data&0x000000FF;
  buffer[10]=writedata.crcpar;
  buffer[11]=writedata.delimend;
  
 // memcpy(buffer, &writedata,sz);
  if (flgDebug)  
  {
    std::string afcc;
    afcc.clear();
    afcc=code+std::to_string(DEBUG)+"FPGA send Big-Endian"+separator+std::to_string(szwrite); 
    for (size_t j = 0; j < szwrite; ++j)
    {
      afcc +=separator + std::to_string(buffer[j]);
    }
    afcc +=endln;
    std::cout << afcc;
    sleep_ms(200);
    afcc.clear();
  }
  while (!uart_is_writable(FPGA_UART_ID)) {sleep_ms(100);} 
  {
    uart_write_blocking(FPGA_UART_ID, buffer,szwrite);
    sleep_ms(30);
    //uart_write_blocking(uart_inst_t *uart, const uint8_t *src, size_t len)
  }

  while (!uart_is_readable(FPGA_UART_ID)) {sleep_ms(100);}
  {
   uart_read_blocking(FPGA_UART_ID, outbuffer,szasc);
  }
//  sleep_ms(200);
 
  if (flgDebug)  
  {
    std::string afcc;
    afcc.clear();
    afcc=code+std::to_string(DEBUG)+"FPGA get Big_endian"+separator+std::to_string(szasc); 
    for (size_t j = 0; j <szasc; ++j)
    {
      afcc +=separator + std::to_string(outbuffer[j]);
    }
    afcc +=endln;
    std::cout << afcc;
    sleep_ms(200);
    afcc.clear();
  }
}

void HARDWARE::set_SetPoint( int32_t SetPoint)
{//  code  22, 2, 8, 0, 1, 0, value
  if (!flgVirtual)
  {
      switch (HARDWAREVERSION)
      {
      case WB:
        dacspt->writeB(SetPoint+ShiftDac);
        break;
      case BB:
        dacspt->writeA(SetPoint+ShiftDac);
        break;       
      case BBFPGA:
        FPGAWriteData writedata;
        writedata.addr=arrModule_0.wbSetpoint;
        writedata.cmd=0x01;
        writedata.data=SetPointScale*(uint32_t)(SetPoint+ShiftDac);  
        WriteDataToFPGA(writedata);
        break;
     }
  } 
  else
  {
    if (HARDWAREVERSION==BBFPGA)
    {
      FPGAWriteData writedata;
      writedata.addr=arrModule_0.wbSetpoint;
      writedata.cmd=0x01;
      writedata.data=SetPointScale*(uint32_t)(SetPoint+ShiftDac);  
      WriteDataToFPGA(writedata);
    }
  }
  // отладка
  if  (flgDebug)
  {
   afc.clear();
   afc =code+std::to_string(DEBUG)+ "debug SetPoint "+ std::to_string(SetPoint);
   afc += endln;
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
   afc =code+std::to_string(DEBUG)+ "debug Ampl Gain "+ std::to_string(gain);
   afc += endln;
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  } 
}
void HARDWARE::set_GainPID(uint32_t gain)
{ 
    uint8_t ti; 
  if (HARDWAREVERSION==BB)  //BB
  {
   uint8_t tiadd;
   ti=(uint8_t)(gain>>8);
   tiadd=(uint8_t)(gain&0x00FF);
   if (!flgVirtual) 
   {  
    std::string binary = std::bitset<3>(ti).to_string();
    binary[2] == '1' ? gainPID0->enable() : gainPID0->disable();
    binary[1] == '1' ? gainPID1->enable() : gainPID1->disable();
    binary[0] == '1' ? gainPID2->enable() : gainPID2->disable();
     /* 
    (ti&0x04) == 1 ? gainPID0->enable() : gainPID0->disable();
    (ti&0x02) == 1 ? gainPID1->enable() : gainPID1->disable();
    (ti&0x01) == 1 ? gainPID2->enable() : gainPID2->disable();
   */ 
    // отладка SPI
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
     afc = code+std::to_string(DEBUG)+"debug PID Gain ti="+ std::to_string(ti)+"ti add="+ std::to_string(tiadd);
    }  
  } //BB 
  else  //add 240603 WB+WBFPGA
  {  
    if (!flgVirtual) 
    { 
     if (HARDWAREVERSION==WB)
     {
      ti=(uint8_t)gain;
      uint8_t intBuf[1]; 
      decoder.activePort(6);
      Spi::setProperties(8, 0, 0);
      intBuf[0] = 0;
      spi_write_blocking(spi_default, intBuf, 1); 
      intBuf[0] = ti;
      spi_write_blocking(spi_default, intBuf, 1); 
      decoder.activePort(7);
     }
     else //Use FPGA
     {
      FPGAWriteData writedata;
      writedata.addr=arrModule_0.wbKx[0];//  0x08430000;  //adress gain need sign
      writedata.cmd=0x01;
      writedata.data=(uint32_t)gain; // gain need sign
      WriteDataToFPGA(writedata);
     }    
    }
    else //virtual
    {
     if (HARDWAREVERSION==BBFPGA) //for test
     {
      FPGAWriteData writedata;
      writedata.addr=arrModule_0.wbKx[0];
      writedata.cmd=0x01;
      writedata.data=(uint32_t)gain; // gain need sign
      WriteDataToFPGA(writedata);
     }
    } 
    if (flgDebug)  
    {
     afc.clear();
     afc = code+std::to_string(DEBUG)+"debug PID Gain WB "+ std::to_string(255-gain); 
    } 
  }  
  if (flgDebug)  
  {
   afc += endln;
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  }  
 }
 void HARDWARE::set_GainPIDFPGA(uint32_t gain)
 { 
    if (!flgVirtual) 
    { 
      FPGAWriteData writedata;
      writedata.addr=arrModule_0.wbKx[0];//  0x08430000;  //adress gain need sign
      writedata.cmd=0x01;
      writedata.data=(uint32_t)gain; // gain need sign
      WriteDataToFPGA(writedata);  
    }
    else //virtual
    {
      FPGAWriteData writedata;
      writedata.addr=arrModule_0.wbKx[0];
      writedata.cmd=0x01;
      writedata.data=(uint32_t)gain; // gain need sign
      WriteDataToFPGA(writedata);
    } 
    if (flgDebug)  
    {
     afc.clear();
     afc = code+std::to_string(DEBUG)+"debug PID Gain FPGA "+ std::to_string(gain); 
    }  
  if (flgDebug)  
  {
   afc += endln;
   std::cout << afc;
   afc.clear();
   sleep_ms(100); 
  }  
 }
void HARDWARE::set_GainPID(uint16_t gain)
{
  uint8_t ti;
  if (HARDWAREVERSION==BB)  //BB
  {
   uint8_t tiadd;
   ti=(uint8_t)(gain>>8);
   tiadd=(uint8_t)(gain&0x00FF);
   if (!flgVirtual) 
   {  
    std::string binary = std::bitset<3>(ti).to_string();
    binary[2] == '1' ? gainPID0->enable() : gainPID0->disable();
    binary[1] == '1' ? gainPID1->enable() : gainPID1->disable();
    binary[0] == '1' ? gainPID2->enable() : gainPID2->disable();
     /* 
    (ti&0x04) == 1 ? gainPID0->enable() : gainPID0->disable();
    (ti&0x02) == 1 ? gainPID1->enable() : gainPID1->disable();
    (ti&0x01) == 1 ? gainPID2->enable() : gainPID2->disable();
   */ 
    // отладка SPI
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
     afc = code+std::to_string(DEBUG)+"debug PID Gain "+ std::to_string(ti)+' '+ std::to_string(tiadd);
    }  
  }  
  else  //add 240603 WB+WBFPGA
  {
    ti=(uint8_t)gain; 
    if (!flgVirtual) 
    { 
     if (HARDWAREVERSION==WB)
     {
      uint8_t intBuf[1]; 
      decoder.activePort(6);
      Spi::setProperties(8, 0, 0);
      intBuf[0] = 0;
      spi_write_blocking(spi_default, intBuf, 1); 
      intBuf[0] = ti;
      spi_write_blocking(spi_default, intBuf, 1); 
      decoder.activePort(7);
     }
     else //Use FPGA
     {
      FPGAWriteData writedata;
      writedata.addr=arrModule_0.wbKx[0];//  0x08430000;  //adress gain need sign
      writedata.cmd=0x01;
      writedata.data=(uint32_t)ti; // gain need sign
      WriteDataToFPGA(writedata);
     }    
    }
    else //virtual
    {
     if (HARDWAREVERSION==BBFPGA)
     {
      FPGAWriteData writedata;
      writedata.addr=arrModule_0.wbKx[0];
      writedata.cmd=0x01;
      writedata.data=(uint32_t)ti; // gain need sign
      WriteDataToFPGA(writedata);
     }
     if (flgDebug)  
     {
      afc.clear();
      afc = code+std::to_string(DEBUG)+"debug PID Gain "+ std::to_string(255-ti); 
     } 
    } 
  }  
  if (flgDebug)  
  {
   afc += endln;
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
  if (HARDWAREVERSION!=BBFPGA)
  { 
   dacz->setSpiProps(); 
   dacz->writeA(int32_t(value)+ShiftDac);
   sleep_us(2);// 240405 
  }
  else //BBFPGA
  {
   FPGAWriteData writedata;
   writedata.addr=arrModule_0.wbOutShift; //?????
   writedata.cmd=0x01;
   writedata.data=(uint32_t)(int32_t(value)+ShiftDac);  
   WriteDataToFPGA(writedata);
  }
}


//uint16_t *getValuesFromAdc();  // чтение АЦП
void HARDWARE::getValuesFromAdc()  // чтение АЦП
{
  if (HARDWAREVERSION!=BBFPGA)
  { 
   repeatTwoTimes();
  // return repeatTwoTimes();
   repeatTwoTimes();
  }
  else
  {
   ReadDataFromFPGAArray();         
  }
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

void HARDWARE::retract() //втянуть
{
 if (HARDWAREVERSION!=BBFPGA)
 {
  protractport->enable();  //  port 6   элемент массива портов 
 }
 else
 {
   FPGAWriteData writedata;
   writedata.addr=arrModule_0.pidControl;
   writedata.cmd=0x01;
   writedata.data=3;  
   WriteDataToFPGA(writedata);
 }

}
void HARDWARE::retract(int16_t HeightJump) //втянуть на HeightJump
{
 retract(); 
 set_DACZ(-abs(HeightJump));
}

void HARDWARE::protract() //вытянуть
{
 if (HARDWAREVERSION!=BBFPGA)
 {
  protractport->disable();  //port 6
 } 
 else
 {
   FPGAWriteData writedata;
   writedata.addr=arrModule_0.pidControl;
   writedata.cmd=0x01;
   writedata.data=1;  
   WriteDataToFPGA(writedata);
 }

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

void HARDWARE::freezeLOOP(uint16_t delay)    // заморозить ПИД
{
 if (HARDWAREVERSION!=BBFPGA)
 {
  freezeport->enable(); // 5 элемент массива портов ???
  sleep_ms(delay);
 }
 else 
 {
   FPGAWriteData writedata;
   writedata.addr=arrModule_0.pidControl;
   writedata.cmd=0x01;
   writedata.data=0;  
   WriteDataToFPGA(writedata);
 }
}

void HARDWARE::unfreezeLOOP(uint16_t delay)  // разморозить ПИД
{
if (HARDWAREVERSION!=BBFPGA)
 {
  freezeport->disable(); // 5 элемент массива портов ???
  sleep_ms(delay);
 }
 else 
 {
   FPGAWriteData writedata;
   writedata.addr=arrModule_0.pidControl;
   writedata.cmd=0x01;
   writedata.data=1;  
   WriteDataToFPGA(writedata);
 }
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