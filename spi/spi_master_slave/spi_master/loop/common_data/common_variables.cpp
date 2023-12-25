#include "common_variables.hpp"

std::string afc;  //dataout 
Spi spi;
LinearDriver linearDriver;
Decoder decoder(4, 5, 6);
Scanner scanner;
DAC8563 dac8563_1(1); // DAC BIAS,SetPoint
DAC8563 dac8563_2(2); // DAC X,Y
//#warning REMOVE STATIC !!! side effects???
uint16_t spiBuf[8];
int32_t  vector[16];  //datain
int vectorSize;

bool STOP = false;   // stop algorithms
bool AD9833_SENDER = false;
bool AD8400_SENDER = false;
bool AD8400_SET_GAIN = false;
bool AD5664 = false;
bool SET_IO_VALUE = false;
bool INIT_ADC;
bool ADC_ENABLE_DISABLE = false;
bool ADC_RESET = false;
bool ADC_READ = false;// чтение сигналов ЦАП
bool ADC_READ_FOREVER = false;
bool ADC_GET_VALUE = false;
bool InitDAC_BIAS_SET_POINT=false;  //DAC8563
bool InitDAC_XY= false;              //DAC8563
bool FREQ_SET  = false;     //AD9833
bool SCANNING  = false;     //сканирование
bool SCAN_CONFIG_UPDATE = false;
bool FASTSCANNING  = false;  //быстрое сканирование
bool SPECTROSOPY_IV= false;
bool MOVE_TOX0Y0   = false;   // сканнер -переместиться в начальную точку X0Y0 скана из начальной точке предыдущего скана
bool LID_MOVE_TOZ0 = false;   // пьезомуавер - отвестись в безопасную начальную точку по Z
bool LID = false;
bool LID_MOVE_UNTIL_STOP  = false; //пьезомуавер - позиционирование X,Y,Z
bool POSXYZ_CONFIG_UPDATE = false;
bool APPROACH = false;       //сближение
bool APPROACH_CONFIG_UPDATE = false;
bool SET_PID_GAIN = false;
bool SET_AMPLMOD_GAIN = false; // усиление модуляции зонда
bool SET_BIAS = false;                //DAC8563
bool SET_SETPOINT = false;            //DAC8563
bool SET_XY = false;                  //DAC8563
bool GET_CURRENTX0Y0 = false;  // текущее состояние сканера  в покое
bool PID_TURN_ON = false;
bool SCANNER_RETRACT = false;
bool SCANNER_PROTRACT = false;
bool SCANNER_RETRACT_PROTRACT = false;

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

//uint32_t DEBUG_LEVEL = 2;
bool Z_STATE = false; //???
bool ADC_IS_READY_TO_READ = true;
bool RESONANCE = false;
bool RESONANCE_STOP = false;
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
