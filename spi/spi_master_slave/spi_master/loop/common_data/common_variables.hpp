#ifndef PICO_EXAMPLES_COMMON_VARIABLES_HPP
#define PICO_EXAMPLES_COMMON_VARIABLES_HPP

#include <pico/critical_section.h>
#include <string>
#include "../../utilities/base_types/Spi.hpp"
#include "../../physical_devices/LinearDriver.hpp"
#include "../../physical_devices/scanner.hpp"
#include "../../devices/DAC8563.hpp"

extern int DEBUG_LEVEL;
extern Spi spi;
extern LinearDriver linearDriver;
extern Decoder decoder;
extern Scanner scanner;
extern DAC8563 dac8563;

extern std::string afc;
extern uint16_t spiBuf[8];
extern int32_t vector[15];
extern int vectorSize;

extern bool AD9833_SENDER;
extern bool AD8400_SENDER;
extern bool AD7606_ENABLE_DISABLE;
extern bool AD7606_RESET;
extern bool AD7606_READ;
extern bool AD7606_READ_FOREVER;
extern bool AD7606_STOP_SCAN;
extern bool AD9833_SET_FREQ;
extern bool AD8400_SET_GAIN;
extern bool AD7606_GET_VALUE;
extern bool AD5664;
extern bool MICRO_SCAN;
extern bool CONFIG_UPDATE;
extern bool MOVE_TO;
extern bool STOP_ALL;
extern bool SET_IO_VALUE;
extern bool SET_ONE_IO_VALUE;
extern bool LID;
extern bool LID_UNTIL_STOP;
extern bool DAC8563_SET_VOLTAGE;
extern bool DAC8563_INIT;
extern bool CONVERGENCE;
extern bool CONVERGENCE_CONFIG_UPDATE;

extern bool Z_STATE;
extern uint16_t ad7606Value;


extern bool AD_7606_IS_READY_TO_READ;
extern bool AD7606_IS_SCANNING;
extern volatile bool is_already_scanning;
extern uint16_t scan_index;
extern uint16_t current_freq;
extern volatile int32_t current_channel;

extern critical_section_t criticalSection;
extern InputPort busy;
extern OutputPort conv;
extern OutputPort dec;
extern OutputPort resetPort;
extern OutputPort ledPort;
extern OutputPort rdbLed;
extern OutputPort io1_0;
extern OutputPort io1_1;
extern OutputPort io2_0;
extern OutputPort io2_1;
extern OutputPort io2_2;
extern OutputPort io3_0;
extern OutputPort io3_1;
extern std::vector<OutputPort> io_ports;

#endif //PICO_EXAMPLES_COMMON_VARIABLES_HPP
