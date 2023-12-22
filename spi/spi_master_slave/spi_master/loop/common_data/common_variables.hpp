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
extern int32_t vector[16];
extern int vectorSize;

//extern bool DAC8563_SET_VOLTAGE_1;
//extern bool DAC8563_SET_VOLTAGE_2;
//extern bool DAC8563_INIT_1;
//extern bool DAC8563_INIT_2;


extern bool AD9833_SENDER;
extern bool AD8400_SENDER;
extern bool AD8400_SET_GAIN;   // AD8400
extern bool AD5664;            //

extern bool ADC_ENABLE_DISABLE;//AD7606
extern bool ADC_RESET;         //AD7606
extern bool ADC_READ;          //AD7606
extern bool ADC_READ_FOREVER;  //AD7606
extern bool ADC_GET_VALUE;     //AD7606
extern bool FREQ_SET;          //AD9833
extern bool SCANNING;
extern bool SPECTROSOPY_IV;
extern bool SCAN_CONFIG_UPDATE;
extern bool MOVE_TOX0Y0; // переместиться в начальную точку  скана из начальной точке предыдущего скана
extern bool LID_MOVE_TOZ0;   // отвестись в безопастную начальную точку по Z
extern bool LID;
extern bool LID_MOVE_UNTIL_STOP;
extern bool SET_IO_VALUE;
extern bool SCANNER_RETRACT_PROTRACT;    //SET_ONE_IO_VALUE;
extern bool APPROACH;
extern bool APPROACH_CONFIG_UPDATE;
extern bool Z_STATE;
// add MF
extern bool STOP;
extern bool FASTSCANNING;
extern bool SET_PID_GAIN;
extern bool SET_AMPLMOD_GAIN;
extern bool InitDAC_BIAS_SET_POINT; //DAC8563
extern bool SET_BIAS;               //DAC8563
extern bool SET_SETPOINT;           //DAC8563
extern bool InitDAC_XY;             //DAC8563
extern bool SET_XY;                 //DAC8563_SET_VOLTAGE_2
extern bool PID_TURN_ON;
extern bool GET_CURRENTX0Y0;
extern bool SCANNER_RETRACT;
extern bool SCANNER_PROTRACT;
extern bool POSXYZ_CONFIG_UPDATE;
extern bool ADC_IS_READY_TO_READ;
extern bool RESONANCE;  //AD9833 
extern bool RESONANCE_STOP;
extern volatile bool RESONANCE_ACTIVE;

extern int16_t ZValue; //??
extern int16_t SignalValue; //??
extern bool    flgVirtual; // Virtual device for debuging
extern uint    flgDebugLevel; //  leveldebug
extern bool    TheadDone;
extern uint8_t ZPin;
extern uint8_t AmplPin; //amplitude
extern uint8_t IPin;    //current
extern int16_t ZMaxValue;
extern int16_t shiftScannerZeroPoint;
//
extern volatile int32_t current_channel;
extern critical_section_t criticalSection;

extern InputPort  busy;
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
