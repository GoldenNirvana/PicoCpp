#include "common_variables.hpp"

std::string afc;
Spi spi;
LinearDriver linearDriver;
Decoder decoder(4, 5, 6);
Scanner scanner;
DAC8563 dac8563;

#warning REMOVE STATIC !!! side effects???
uint16_t spiBuf[8];
int32_t vector[15];
int vectorSize;

bool AD9833_SENDER = false;
bool AD8400_SENDER = false;
bool AD7606_ENABLE_DISABLE = false;
bool AD7606_RESET = false;
bool AD7606_READ = false;
bool AD7606_READ_FOREVER = false;
bool AD7606_STOP_SCAN = false;
bool AD9833_SET_FREQ = false;
bool AD8400_SET_GAIN = false;
bool AD7606_GET_VALUE = false;
bool AD5664 = false;
bool MICRO_SCAN = false;
bool CONFIG_UPDATE = false;
bool MOVE_TO = false;
bool STOP_ALL = false;
bool SET_IO_VALUE = false;
bool SET_ONE_IO_VALUE = false;
bool LID = false;
bool LID_UNTIL_STOP = false;
bool AD7606_TRIG_GET_VALUE = false;
bool AD7606_GET_ALL_VALUES = false;
bool DAC8563_SET_VOLTAGE = false;
bool DAC8563_INIT = false;
bool CONVERGENCE = false;
bool CONVERGENCE_CONFIG_UPDATE = false;


bool Z_STATE = false;
uint16_t ad7606Value = 0;

bool AD_7606_IS_READY_TO_READ = true;
bool AD7606_IS_SCANNING = false;
volatile bool is_already_scanning = false;
uint16_t scan_index = 0;
uint16_t current_freq = 0;
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
OutputPort io3_0(26);
OutputPort io3_1(27);

std::vector<OutputPort> io_ports;
