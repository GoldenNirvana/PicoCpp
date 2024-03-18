#include "common_variables.hpp"

std::string afc;  //dataout string
Spi spi;
LinearDriver linearDriver;
Decoder decoder(4, 5, 6);
Scanner scanner;

//int32_t  vector[16];  //datain
std::vector<int32_t> vector;
std::vector<int32_t> vupdateparams;

int32_t vectorSize;

DAC8563 dac8563_1(1); // DAC BIAS,SetPoint
DAC8563 dac8563_2(2); // DAC X,Y
DAC8563 dac8563_3(1); // DAC Z
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
InputPort  busy(16); // FIXME TEMP!!!
OutputPort conv(7);
OutputPort dec(10);
OutputPort resetPort(17); // FIXME TEMP
OutputPort ledPort(PICO_DEFAULT_LED_PIN);
OutputPort rdbLed(23);
OutputPort io1_0(11); 
OutputPort io1_1(12);
OutputPort io2_0(13);
OutputPort io2_1(14);
OutputPort io2_2(15);
OutputPort io3_0(26); //вытянуть сканнер
OutputPort io3_1(27); //втянуть сканнер

std::vector<OutputPort> io_ports;
