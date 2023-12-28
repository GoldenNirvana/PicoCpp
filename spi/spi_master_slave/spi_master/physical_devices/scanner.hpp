#ifndef PICO_EXAMPLES_SCANNER_HPP
#define PICO_EXAMPLES_SCANNER_HPP

#include <vector>
#include "../utilities/base_types/Point.hpp"

struct Config
{
  uint16_t nPoints_x;        // точек по оси  X                              1
  uint16_t nPoints_y;        // точек по оси  Y                              2 
  uint8_t  path;             // сканирование  0 - по оси X, 1 - по оси Y     3
  uint8_t  method;           // что измерять Topo=0,Phase=1, Ampl=2...       4
  uint16_t delayF;           // задержка при сканировании вперёд             5
  uint16_t delayB;           // задержка при сканировании назад              6
  uint16_t betweenPoints_x;  // расстояние между точками по X в дискретах    7 
  uint16_t betweenPoints_y;  // расстояние между точками по Y в дискретах    8 
  uint8_t  size;             // size=1  -Z; size=2 - Z,Амплитуда             9
  uint8_t  Ti;               // усиление ПИД                                10
  uint16_t diskretinstep;    // размер шага в дискретах                     11
  uint16_t pause;            // время ожидания в точке измерения  мксек     12
  uint8_t  flgOneFrame;      // быстрое сканирование один кадр=1            13
  uint8_t  flgHoping;        // сканирование прыжками                       14
  uint16_t HopeDelay;        // задержка в точке измерения при прыжках      15
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

public:

  Scanner();

  ~Scanner();

  void start_scan();

  void start_hopingscan();  //сканирование прыжками

  void start_fastscan();    // быстрое сканирование и вывод скана целиком, а не по линиям

  void approacphm(const int16_t *const data);  // захват взаимодействия с контролем ворот

  void positioningXYZ(const int16_t *const data); // abs(n) перемещение по  X,Y и Z (с контроллем ворот)

  void start_frqscan(); //поиск резонанса датчика

  void spectroscopyIV(const int32_t *const data); // спектроскопия I-V

  void spectroscopyAZ(const int32_t *const data); // спектроскопия Ampl-Z

  void scan_update(const Config &config);  // обновить параметры скaнирования

  void move_toX0Y0(int x, int y, int delay); //переместиться в начальную точку скана из начальной точке предыдущего скана

  void LID_move_toZ0(int lid_name, int f, int p, int n, int dir);// отвестись в безопасную начальную точку по Z при старте и выходе из программы

  void retract();       // втянуть сканер

  void protract();      // втянуть сканер

  void freezeLOOP();    // заморизить ПИД

  void unfreezeLOOP();  // разморизить ПИД

  bool getHoppingFlg(); // получить флаг сканирования прыжками

  Point getX0Y0();      // получить текущую точку сканера в покое

private:

  std::vector<uint16_t> vector_z, other_info,vectorI_V,vectorA_Z;
  Point pos_, prev_point;
  Config conf_;
};

#endif
