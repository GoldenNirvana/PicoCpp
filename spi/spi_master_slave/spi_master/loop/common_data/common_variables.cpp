#include "common_variables.hpp"

//////////////////////////////////////////////
std::string  SOFTVERSION="24.08.21.1 HWA_FPGA_UART";
std::string  SoftHARDWAREVERSION="0.1";
int8_t       HARDWAREVERSION=1;   // PICO(BB)=0 ; MotherBoard(WhiteBoard=WB)=1; BlueBoard+FPGA(BBFPGA)=2 ПЛИС;
//int8_t       HARDWAREVERSION_I=-1;
std::string afc;  //dataout string
std::vector<int32_t> vector;
std::vector<int32_t> vupdateparams;
int32_t vectorSize;
uint16_t spiBuf[8];
std::atomic<int16_t>     ALGCODE;
std::atomic<bool>  CONFIG_UPDATE;
std::atomic<bool>  STOP;
std::atomic<bool>  TheadDone;   //need dor synchronization with PC 
std::atomic<bool>  DrawDone;    //need dor synchronization with PC 
std::atomic<bool>  ADC_IS_READY_TO_READ = true;
std::atomic<bool>  flgVirtual = false; // start value!!! setPIDGain
std::atomic<bool>  flgDebug   = false; // strat value!!!

const std::string  code="code";  
const std::string  endln="\n";
const char         separator=',';

bool AD9833_SENDER = false;
bool AD8400_SENDER = false;
bool AD8400_SET_GAIN = false;
bool AD5664 = false;
bool SET_IO_VALUE = false;
bool ADC_ENABLE_DISABLE = false;
//bool ADC_RESET = false;
bool ADC_READ_FOREVER = false;
bool ADC_GET_VALUE = false;
//************************************************

bool    flgUseTUD=false;
bool    flgParamsUpdated=false;
bool    flgСritical_section=true;
uint8_t flgDebugLevel = 3;      // уровень отладки
bool    flgUseUART = false;     // использовать UART для передачи данных

//данные для симуляции 
int16_t ZValue      = maxint16_t;
int16_t SignalValue = maxint16_t;
int16_t ZMaxValue   = maxint16_t;
int16_t SignalMaxValue   = maxint16_t; //add 240710
int32_t ShiftDac    = 32768;
int8_t  SetPointScale= 1;
uint8_t ZPin    = 0; // Z
uint8_t AmplPin = 1; // амплитуда
uint8_t IPin    = 2; // ток  
bool Z_STATE = false; //???

critical_section_t criticalSection;
