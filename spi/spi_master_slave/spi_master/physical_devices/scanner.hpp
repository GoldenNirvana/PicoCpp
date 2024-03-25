#ifndef PICO_EXAMPLES_SCANNER_HPP
#define PICO_EXAMPLES_SCANNER_HPP

#include <string>
#include <vector>
#include <random>
#include "../utilities/base_types/Point.hpp"

struct Config
{
  uint16_t nPoints_x;        // точек по оси  X                                            1
  uint16_t nPoints_y;        // точек по оси  Y                                            2 
  uint8_t  path;             // сканирование  0 - по оси X, 1 - по оси Y                   3
  uint8_t  method;           // что измерять Topo=0,Phase=1, Ampl=2...                     4
  uint16_t delayF;           // задержка при сканировании вперёд                           5
  uint16_t delayB;           // задержка при сканировании назад                            6
  uint16_t betweenPoints_x;  // расстояние между точками по X в дискретах                  7 
  uint16_t betweenPoints_y;  // расстояние между точками по Y в дискретах                  8 
  uint8_t  size;             // size=1  -Z; size=2 - Z,Амплитуда                           9
  uint8_t  Ti;               // усиление ПИД                                              10
  uint16_t diskretinstep;    // размер шага в дискретах                                   11
  uint16_t pause;            // время ожидания в точке измерения  мксек                   12  
  uint8_t  flgLin;           // флаг линеализации                                         13   
  int16_t  lineshift;        //сдвиг линии -учет неортогональности сканнера               14
  uint8_t  flgOneFrame;      // быстрое сканирование один кадр=1                          15
  uint8_t  flgHoping;        // сканирование прыжками                                     16
  uint16_t HopeDelay;        // задержка в точке измерения при прыжках                    17
  uint16_t  HopeZ;            // прыжок по Z,если=0,то прыжок по максимуму                 18
  uint8_t  flgAutoUpdateSP;   // автообновление опоры на каждой линии                     19
  uint8_t  flgAutoUpdateSPDelta;// обновление опоры , если изменение тока превысило порог 20
  uint16_t ThresholdAutoUpdate;//изменения опоры, если изменение тока превысило порог     21
  uint16_t KoeffCorrectISat;    // опора  %  от тока насыщения                            22
  int16_t  SetPoint;            // опора  ток                                             23
};



struct ConfigCurrent
{
  uint16_t delayF;  // Задержка вперёд
  uint16_t delayB;  // Задержка назад
  uint8_t  Ti;      // Усиление ПИД
};

class Scanner
{

private:
  
  void stop_scan();         // возвращение сканера в  начальную точку скана
  
  void move_to(const Point &point, uint16_t delay);  // переместиться в начальную точку скана текущего скана

  void sendStrData(std::string const& header,std::vector<int32_t>  &data, const uint16_t delay,const bool flg);
  void sendStrData(std::string const& header,std::vector<uint16_t> &data, const uint16_t delay,const bool flg);
  void sendStrData(std::string const& header,std::vector<int16_t>  &data, const uint16_t delay,const bool flg); //flg clear data
  void sendStrData(std::string const& header);

public:

   Scanner();

  ~Scanner();

  void start_scan(std::vector<int32_t> &vector);
  
  void start_scanlin(std::vector<int32_t> &vector);

  void start_hopingscan(std::vector<int32_t> &vector); //сканирование прыжками
  
  void start_hopingscanlin(std::vector<int32_t> &vector); //сканирование прыжками

  void start_fastscan(std::vector<int32_t> &vector);  // быстрое сканирование и вывод скана целиком, а не по линиям

  void approacphm(std::vector<int32_t> &vector);      // захват взаимодействия с контролем ворот

  void positioningXYZ(std::vector<int32_t> &vector);  // abs(n) перемещение по  X,Y и Z (с контролем ворот)

  void start_frqscan();                               //поиск резонанса датчика
  
  void testpiezomover(std::vector<int32_t> &vector);  //тестирование шагов пьезомувера

  void spectroscopyIV(std::vector<int32_t> &vector);  // спектроскопия I-V

  int16_t ZMove( int16_t Z0, int16_t dZ, int16_t stepsize, uint16_t delay );   // stepsize=+-1  sign  -> dir 

  void spectroscopyAIZV0(std::vector<int32_t> &vector); // спектроскопия Ampl-Z

  void spectroscopyAIZ(std::vector<int32_t> &vector);

  void scan_update(const Config &config);             // обновить параметры скaнирования

  void move_toX0Y0(uint16_t x, uint16_t y, uint16_t delay, int8_t flg);          //переместиться в начальную точку скана из начальной точке предыдущего скана

  void LID_move_toZ0(int lid_name, int f, int p, int n, int dir);// отвестись в безопасную начальную точку по Z при старте и выходе из программы
  
  void readADC();

  void readDATALin();
  
  void scanner_retract_protract(int port, int flg);
  
  void retract();       // втянуть сканер

  void retract(int16_t HeightJump); //втянуть на H

  void protract();      // втянуть сканер
 
  void protract(uint16_t delay,int16_t DacZ0,int16_t HeightJump) ; //разморозить ПИД 
 
  void LOOP_freeze_unfreeze(int port, int flg);  
 
  void freezeLOOP(uint16_t delay);    // заморизить ПИД

  void unfreezeLOOP(uint16_t delay);  // разморизить ПИД

  bool getHoppingFlg(); // получить флаг сканирования прыжками
 
  bool getLinearFlg();  // сканирование с линеализацией
  
  Point getX0Y0();      // получить текущую точку сканера в покое

private:

  std::vector<uint16_t> vector_data;
  std::vector<uint16_t> data_LinX;
  std::vector<uint16_t> data_LinY;
  std::vector<int32_t>  vectorA_Z;
  std::vector<int16_t>  vectorI_V;
  std::vector<int32_t>  debugdata; 
  Point pos_, prev_point;
  Config conf_;
};

#endif
