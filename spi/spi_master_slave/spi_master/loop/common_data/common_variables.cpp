#include "common_variables.hpp"

//////////////////////////////////////////////
std::string  SOFTVERSION="24.05.29.01 HWA MB pointer";
std::string  HARDWAREVERSION="1.1";
int8_t       HARDWAREVERSION_I=0;
//int8_t       HARDWAREVERSION_I=-1;
// std::string  HARDWAREVERSION="2.0"; //new hardware
std::string afc;  //dataout string
std::vector<int32_t> vector;
std::vector<int32_t> vupdateparams;
int32_t vectorSize;
uint16_t spiBuf[8];
std::atomic<int16_t>      ALGCODE;
std::atomic<bool> STOP = false;   // cmd stop algorithms
std::atomic<bool> TheadDone = false;
std::atomic<bool> DrawDone=true;
std::atomic<bool> ADC_IS_READY_TO_READ = true;
std::atomic<bool> CONFIG_UPDATE = false;
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
bool    flgParamsUpdated=false;
bool    flgСritical_section=true;
bool    flgDebug=false;
bool    flgVirtual = false;     // флаг симуляции работа микроконтроллера
uint8_t flgDebugLevel = 3;      // уровень отладки
bool    flgUseUART = false;     // использовать UART для передачи данных
//данные для симуляции 
int16_t ZValue      = maxint16_t;
int16_t SignalValue = maxint16_t;
int16_t ZMaxValue   = maxint16_t;
int32_t ShiftDac    = 32768;

uint8_t ZPin    = 0; // Z
uint8_t AmplPin = 1; // амплитуда
uint8_t IPin    = 2; // ток  
bool Z_STATE = false; //???

critical_section_t criticalSection;
