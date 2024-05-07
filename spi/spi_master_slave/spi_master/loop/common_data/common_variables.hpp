#ifndef PICO_EXAMPLES_COMMON_VARIABLES_HPP
#define PICO_EXAMPLES_COMMON_VARIABLES_HPP

#include <pico/critical_section.h>
#include <string>
#include <vector>

#define SFM                      0
#define STM                      1
#define SICMDC                   3  
#define maxint16_t               32767
#define minint16_t              -32768

#define ALGNONE                   0
#define VersionCmd               10
#define ChangeHardWare           11
#define ADC_READCmd              12  //AD7606 timer
#define DebugCmd                 13
#define VirtualCmd               14
#define DebugLevelCmd            15
#define DEBUG                    16
#define END                      17
#define GET_CURRENTX0Y0          18
#define SET_BIAS                 19
#define SET_SETPOINT             20  //DAC8563_1
#define SET_PID_GAIN             21
#define SET_AMPLMOD_GAIN         22
#define InitDAC_BIAS_SET_POINT   23  //DAC8563_1 
#define ADC_GET_VALUECmd         24  // don't use
#define SetDACZeroCmd            25
#define InitDAC_Z                26  //DAC8563_3
#define InitDAC_XY               27  //DAC8563_2
#define SET_Z                    28  //DAC8563_3
#define SET_XY                   29  //DAC8563_SET_VOLTAGE_2
#define FREQ_SET                 30  //AD9833
#define STOPPED                  31
#define PARAMUPDATEDCmd          32
#define TheadDoneCmd             33
#define SetUseCritialSectAlgCode 34
#define RetractAlgCode           35
#define RESONANCE                40  //AD9833 
#define SCANNING                 50
#define MOVE_TOX0Y0              51  //переместиться в начальную точку  скана из начальной точке предыдущего скана
#define CONFIG_UPDATECmd         55
#define FASTSCANNING             56
#define SENDDATALIN              57  //данные линеализации
#define ProtractAlgCode          61
#define FreezePIDAlgCode         62
#define UnFreezePIDAlgCode       63
#define SPECTROSOPY_IV           65
#define SPECTROSOPY_AIZ          66
#define STOPCmd                  70
#define DRAWDONECmd              71
#define APPROACH                 75
#define TESTMOVER                76
#define LID_MOVE_UNTIL_STOP      80
#define LID_MOVE_TOZ0            84 // отвестись в безопасную начальную точку по Z
#define portx                    0
#define porty                    1

extern std::string  SOFTVERSION;
extern std::string  HARDWAREVERSION;  ///  {0,1,....} or {0.1, 1.1...} 
extern int16_t      ALGCODE;
extern uint32_t     DEBUG_LEVEL;


extern std::string afc;
extern uint16_t spiBuf[8];

extern std::vector<int32_t> vector;
extern std::vector<int32_t> vupdateparams;
extern int32_t vectorSize;

extern bool AD9833_SENDER;
extern bool AD8400_SENDER;
extern bool AD8400_SET_GAIN;   // AD8400
extern bool AD5664;            //

extern bool ADC_ENABLE_DISABLE;//AD7606
extern bool ADC_RESET;         //AD7606     
extern bool ADC_READ_FOREVER;  //AD7606
extern bool ADC_GET_VALUE;     //AD7606

extern bool LID;
extern bool SET_IO_VALUE;
extern bool CONFIG_UPDATE;
extern bool Z_STATE;
// add MF
extern bool LOOP_FREEZE_UNFREEZE;
extern bool STOP;
extern bool PID_TURN_ON;
extern bool ADC_IS_READY_TO_READ;
// service flags
extern bool TheadDone;   //need dor synchronization with PC 
extern bool DrawDone;
extern bool flgParamsUpdated;
extern bool flgDebug;
extern bool flgVirtual;  // Virtual device for debuging and simulation
extern bool flgСritical_section;
extern bool flgUseUART;  //
extern uint8_t ZPin;
extern uint8_t AmplPin; //amplitude
extern uint8_t IPin;    //current
extern uint8_t flgDebugLevel; //  leveldebug
extern int16_t ZValue; //for simulation
extern int16_t SignalValue; //for simulation //  
extern int16_t ZMaxValue;
extern int32_t ShiftDac;

//extern volatile int32_t current_channel;
extern critical_section_t criticalSection;
#endif //PICO_EXAMPLES_COMMON_VARIABLES_HPP
