#include "common_variables.hpp"

std::string afc;
Spi spi;
LinearDriver linearDriver;
Decoder decoder(4, 5, 6);
Scanner scanner;
DAC8563 dac8563_1(1);
DAC8563 dac8563_2(2);
//#warning REMOVE STATIC !!! side effects???
uint16_t spiBuf[8];
int32_t vector[16];
int vectorSize;

bool AD9833_SENDER = false;
bool AD8400_SENDER = false;
bool AD8400_SET_GAIN = false;
bool AD5664 = false;
bool SET_IO_VALUE = false;
bool ADC_ENABLE_DISABLE = false;
bool ADC_RESET = false;
bool ADC_READ = false;
bool ADC_READ_FOREVER = false;
bool ADC_GET_VALUE = false;
bool SCANNER_RETRACT_PROTRACT = false;
bool INIT_ADC;
bool READ_ADC;  // чтение сигналов ЦАП
bool FREQ_SET = false;     //AD9833
bool SCANNING = false;     //сканирование
bool SPECTROSOPY_IV=false;
bool CONFIG_UPDATE = false;
bool MOVE_TOX0Y0 = false; //переместиться в начальную точку X0Y0 скана из начальной точке предыдущего скана
bool STOP_ALL = false;
bool LID = false;
bool LID_UNTIL_STOP = false; //позиционирование
bool APPROACH = false;       //сближение
bool APPROACH_CONFIG_UPDATE = false;
bool SET_PID_GAIN=false;
bool SET_AMPLMOD_GAIN=false; // усиление модуляции зонда
bool SET_BIAS=false;                //DAC8563
bool SET_SETPOINT=false;            //DAC8563
bool InitDAC_BIAS_SET_POINT=false;  //DAC8563
bool InitDAC_XY=false;              //DAC8563
bool SET_XY=false;                  //DAC8563
bool FASTSCANNING = false;  //быстрое сканирование
bool POSXYZ_CONFIG_UPDATE = false;
bool GET_CURRENTX0Y0 = false;
bool MOVE_TOZ0 = false;   // отвестись в безопастную начальную точку по Z
bool PID_TURN_ON = false;
bool SCANNER_RETRACT = false;
bool SCANNER_PROTRACT = false;


bool flgVirtual = false; // флаг симуляции работа микроконтроллера
uint flgDebugLevel = 2; //  уровень отладки
//данные для симуляции 
int16_t ZValue      = 32767;
int16_t SignalValue = 32767;
int16_t ZMaxValue   = 32767;
bool TheadDone = false;
uint8_t ZPin    = 0; // Z
uint8_t AmplPin = 1; // амплитуда
uint8_t IPin    = 2; // ток  

int16_t shiftScannerZeroPoint = -32768;

//uint32_t DEBUG_LEVEL = 2;
bool Z_STATE = false;
bool ADC_IS_READY_TO_READ = true;
bool RESONANCE = false;
bool RESONANCE_STOP = false;
volatile bool RESONANCE_ACTIVE = false;
volatile int32_t current_channel = 0;

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
