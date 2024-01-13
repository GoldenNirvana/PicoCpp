#ifndef PICO_EXAMPLES_COMMON_VARIABLES_HPP
#define PICO_EXAMPLES_COMMON_VARIABLES_HPP

#include <pico/critical_section.h>
#include <string>
#include "../../utilities/base_types/Spi.hpp"
#include "../../physical_devices/LinearDriver.hpp"
#include "../../physical_devices/scanner.hpp"
#include "../../devices/DAC8563.hpp"

#define RESONANCE               25  //AD9833 
#define APPROACH                75
#define FREQ_SET                30 //AD9833
#define SCANNING                50
#define FASTSCANNING            56
#define MOVE_TOX0Y0             51 //переместиться в начальную точку  скана из начальной точке предыдущего скана
#define LID_MOVE_TOZ0           84 // отвестись в безопасную начальную точку по Z
#define LID_MOVE_UNTIL_STOP     80
#define SET_PID_GAIN            60
#define InitDAC_BIAS_SET_POINT  23  //DAC8563
#define InitDAC_XY              27  //DAC8563
#define SET_SETPOINT            22  //DAC8563
#define SET_XY                  29  //DAC8563_SET_VOLTAGE_2
#define ADC_READ                12  //AD7606
//#define ADC_GET_VALUE  ;
//#define SCANNER_RETRACT
//#define SCANNER_PROTRACT
#define GET_CURRENTX0Y0          18
#define SCANNER_RETRACT_PROTRACT 61
#define SPECTROSOPY_IV           65
#define SPECTROSOPY_AZ           66
#define SET_BIAS                 19
#define ALGNONE                  0

extern int16_t ALGCODE;

extern uint32_t DEBUG_LEVEL;
extern Spi spi;
extern LinearDriver linearDriver;
extern Decoder decoder;
extern Scanner scanner;
extern DAC8563 dac8563_1;  // DAC BIas,SetPoint
extern DAC8563 dac8563_2;  // DAC X,Y

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
extern bool ADC_READ_FOREVER;  //AD7606
extern bool ADC_GET_VALUE;     //AD7606
extern bool SCAN_CONFIG_UPDATE;
extern bool LID;
extern bool SET_IO_VALUE;
extern bool CONFIG_UPDATE;
extern bool APPROACH_CONFIG_UPDATE;
extern bool Z_STATE;
// add MF
extern bool LOOP_FREEZE_UNFREEZE;
extern bool STOP;
extern bool SET_AMPLMOD_GAIN;
extern bool PID_TURN_ON;
extern bool SCANNER_RETRACT;
extern bool SCANNER_PROTRACT;
extern bool POSXYZ_CONFIG_UPDATE;
extern bool ADC_IS_READY_TO_READ;
extern bool RESONANCE_STOP; //???
extern bool TheadDone;   //need dor synchronization with PC 
extern uint8_t ZPin;
extern uint8_t AmplPin; //amplitude
extern uint8_t IPin;    //current

extern int16_t ZValue; //for simulation
extern int16_t SignalValue; //for simulation
extern bool    flgVirtual;  // Virtual device for debuging and simulation
extern uint    flgDebugLevel; //  leveldebug
extern int16_t ZMaxValue;
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
