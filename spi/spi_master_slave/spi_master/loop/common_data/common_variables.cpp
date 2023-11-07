#include "common_variables.hpp"

std::string afc;
Spi spi;
LinearDriver linearDriver;
Decoder decoder(4, 5, 6);
Scanner scanner;
DAC8563 dac8563;

//#warning REMOVE STATIC !!! side effects???
uint16_t spiBuf[8]; // z, ampl ?
int32_t vector[15]; //int32_t  ?? int16_t
int vectorSize;

bool AD9833_SENDER = false;
bool AD8400_SENDER = false;
bool AD7606_ENABLE_DISABLE = false;
bool AD7606_RESET = false;
bool AD7606_READ = false;
bool AD7606_READ_FOREVER = false;
bool FREQ_SET = false; //AD9833
bool AD8400_SET_GAIN = false;
bool AD7606_GET_VALUE = false;
bool AD5664 = false;
bool SCANNING = false;
bool CONFIG_UPDATE = false;
bool MOVE_TO = false;
bool STOP_ALL = false;
bool SET_IO_VALUE = false;
bool SET_ONE_IO_VALUE = false;
bool LID = false;
bool LID_UNTIL_STOP = false;
bool DAC8563_SET_VOLTAGE = false;
bool DAC8563_INIT = false;
bool APPROACH = false;
bool APPROACH_CONFIG_UPDATE = false;
//add Mf
bool flgVirtual =false; // Virtual device for debuging
uint flgDebugLevel =2; //  leveldebug
bool SET_PID_GAIN=false;
bool PID_TURN_ON=false;
bool Scanner_Retract=false;
bool Scanner_Protract=false;
bool POSXYZ_CONFIG_UPDATE=false;
uint16_t ZValue = 0;
uint16_t SignalValue = 0;
bool TheadDone=false;
//

//uint32_t DEBUG_LEVEL = 2;
bool Z_STATE = false;
bool AD_7606_IS_READY_TO_READ = true;
bool RESONANCE = false;
bool RESONANCE_STOP = false;
volatile bool RESONANCE_ACTIVE = false;
//uint16_t scan_index = 0;
//uint16_t current_freq = 0;
volatile int32_t current_channel = 0;

critical_section_t criticalSection;
 InputPort busy(16); // FIXME TEMP!!!
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
