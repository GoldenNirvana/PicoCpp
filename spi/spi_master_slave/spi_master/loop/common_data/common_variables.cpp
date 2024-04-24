#include "common_variables.hpp"

//////////////////////////////////////////////
std::string  SOFTVERSION="24.04.19.01";;
std::string  HARDWAREVERSION="1.0";
// std::string  HARDWAREVERSION="2.0"; //new hardware
//////////////////////////////////////////////

std::string afc;  //dataout string
Spi spi;
//LinearDriver linearDriver; //24/04/24
//Decoder decoder(4, 5, 6); //24/04/24

std::vector<int32_t> vector;
std::vector<int32_t> vupdateparams;

int32_t vectorSize;

//#warning REMOVE STATIC !!! side effects???
uint16_t spiBuf[8];

int16_t ALGCODE=0;   
bool STOP = false;   // cmd stop algorithms
bool AD9833_SENDER = false;
bool AD8400_SENDER = false;
bool AD8400_SET_GAIN = false;
bool AD5664 = false;
bool SET_IO_VALUE = false;
bool ADC_ENABLE_DISABLE = false;
bool ADC_RESET = false;
bool ADC_READ_FOREVER = false;
bool ADC_GET_VALUE = false;
bool CONFIG_UPDATE = false;
bool LID = false;
bool PID_TURN_ON = false;
bool SCANNER_RETRACT = false;
bool SCANNER_PROTRACT = false;
bool LOOP_FREEZE_UNFREEZE=false;
bool TheadDone = false;
bool DrawDone=true;


//************************************************
bool    flgParamsUpdated=false;
bool    flgСritical_section=true;
bool    flgDebug=false;
bool    flgVirtual = false;     // флаг симуляции работа микроконтроллера
uint8_t flgDebugLevel = 2;      // уровень отладки
bool    flgUseUART = false;     // использовать UART для передачи данных
//данные для симуляции 
int16_t ZValue      = maxint16_t;
int16_t SignalValue = maxint16_t;
int16_t ZMaxValue   = maxint16_t;
int32_t ShiftDac    = 32768;

uint8_t ZPin    = 0; // Z
uint8_t AmplPin = 1; // амплитуда
uint8_t IPin    = 2; // ток  

//uint32_t DEBUG_LEVEL = 2;
bool Z_STATE = false; //???
bool ADC_IS_READY_TO_READ = true;
//volatile int32_t current_channel = 0;

critical_section_t criticalSection;
