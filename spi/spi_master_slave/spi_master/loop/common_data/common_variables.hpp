#ifndef PICO_EXAMPLES_COMMON_VARIABLES_HPP
#define PICO_EXAMPLES_COMMON_VARIABLES_HPP

#include <pico/critical_section.h>
#include <string>
#include "../../utilities/base_types/Spi.hpp"
#include "../../physical_devices/LinearDriver.hpp"
#include "../../physical_devices/scanner.hpp"
#include "../../devices/DAC8563.hpp"

extern uint32_t DEBUG_LEVEL;
extern Spi spi;
extern LinearDriver linearDriver;
extern Decoder decoder;
extern Scanner scanner;
extern DAC8563 dac8563_1;
extern DAC8563 dac8563_2;

extern std::string afc;
extern uint16_t spiBuf[8];
extern int32_t vector[15];
extern int vectorSize;

extern bool DAC8563_SET_VOLTAGE_1;
extern bool DAC8563_SET_VOLTAGE_2;
extern bool DAC8563_INIT_1;
extern bool DAC8563_INIT_2;


extern bool AD9833_SENDER;
extern bool AD8400_SENDER;
//
extern bool AD7606_ENABLE_DISABLE;
extern bool AD7606_RESET;
extern bool AD7606_READ;
extern bool AD7606_READ_FOREVER;
extern bool AD7606_GET_VALUE;
//
extern bool FREQ_SET;        // AD9833
extern bool AD8400_SET_GAIN; // AD8400
extern bool AD5664;
extern bool SCANNING;
extern bool CONFIG_UPDATE;
extern bool MOVE_TOX0Y0; // переместиться в начальную точку  скана из начальной точке предыдущего скана
extern bool MOVE_TOZ0;   // отвестись в безопастную начальную точку по Z
extern bool STOP_ALL;
extern bool SET_IO_VALUE;
extern bool SET_ONE_IO_VALUE;
extern bool LID;
extern bool LID_UNTIL_STOP;

extern bool APPROACH;
extern bool APPROACH_CONFIG_UPDATE;
extern bool Z_STATE;
// add MF
extern int16_t ZValue; //??
extern int16_t SignalValue; //??
extern bool flgVirtual; // Virtual device for debuging
extern uint flgDebugLevel; //  leveldebug
extern bool SET_PID_GAIN;
extern bool PID_TURN_ON;
extern bool GET_CURRENTX0Y0;
extern bool Scanner_Retract;
extern bool Scanner_Protract;
extern bool POSXYZ_CONFIG_UPDATE;
extern bool TheadDone;
extern uint8_t ZPin;
extern uint8_t SignalPin;
extern int16_t ZMaxValue;
extern int16_t shiftScannerZeroPoint;
//
extern bool AD_7606_IS_READY_TO_READ;
extern bool RESONANCE;  //AD9833 
extern bool RESONANCE_STOP;
extern volatile bool RESONANCE_ACTIVE;
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
