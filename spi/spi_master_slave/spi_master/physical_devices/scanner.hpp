#ifndef PICO_EXAMPLES_SCANNER_HPP
#define PICO_EXAMPLES_SCANNER_HPP

#include <vector>
#include "../utilities/base_types/Point.hpp"

struct Config
{
  uint16_t nPoints_x;        // Точек по линии X                             1
  uint16_t nPoints_y;        // Точек по линии Y                             2 
  uint8_t  path;             // скан  0 - по X, 1 - по Y                     3
  uint8_t  method;           // Что измерять Topo=0,Phase=1, Ampl=2...       4
  uint16_t delayF;           // Задержка вперёд                              5
  uint16_t delayB;           // Задержка назад                               6
  uint16_t betweenPoints_x;  // Расстояние между точками по X дискрет        7 
  uint16_t betweenPoints_y;  // Расстояние между точками по Y дискрет        8 
  uint8_t  size;             // size=1 Z; size=2 Z,Signal add signal         9
  uint8_t  Ti;               // PID Gain                                    10
  uint16_t diskretinstep;    // размер шага в дискрет                       11
  uint16_t pause;            // ms? время ожидания в точке измерения        12
  uint8_t  flgOneFrame;      // need for Fast Scanning  =1 only one frame   13
  uint8_t  flgHoping;        // Hoping;                                     14
  uint16_t HopeDelay;        // delay Hoping;                               15
};


struct ConfigCurrent
{
  uint16_t delayF;  // Задержка вперёд
  uint16_t delayB;  // Задержка назад
  uint8_t  Ti;      // PID Gain
};

class Scanner
{
public:

  Scanner();

  ~Scanner();

  void start_scan();

  void start_hopingscan();  //сканирование прыжками

  void start_fastscan();    // быстрое сканирование и вывод скана целиком, а не по линиям

  void stop_scan();   // возвращение сканера в  начальную точку скана

  void update(const Config &config); // обновить параметры скнирования

  void move_to(const Point &point, uint16_t delay);  // переместиться в нач.точку скана текущего скана

  void move_toX0Y0(int x, int y, int delay); //переместиться в начальную точку скана из начальной точке предыдущего скана
  //add mf 
  Point getX0Y0(); // получить текущую точку сканера в покое

  void move_toZ0(int lid_name, int f, int p, int n, int dir);// отвестись в безопастную начальную точку по Z при старте и выходе из программы

  void retract(); //втянуть сканер

  void protract(); //втянуть сканер

  void approacphm(const int16_t *const data);  // захват взаимодействия с контролем ворот

  void positioningXYZ(int lid_name, int f, int p, int n, int dir, int16_t gtmax, int16_t gtmin); // abs(n) перемещение по  X,Y и Z (с контроллем ворот)

  void start_frqscan(); //find resonance

  bool getHoppingFlg(); // получить флаг сканирования прыжками

  // void stopAll();
//
private:

  std::vector<uint16_t> vector_z, other_info;
  Point pos_, prev_point;
  Config conf_;
};

#endif
