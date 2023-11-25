#ifndef PICO_EXAMPLES_SCANNER_HPP
#define PICO_EXAMPLES_SCANNER_HPP

#include <vector>
#include "../utilities/base_types/Point.hpp"

struct Config
{
  uint16_t nPoints_x;        // Точек по линии X
  uint16_t nPoints_y;        // Точек по линии Y
   uint8_t path;             // скан  0 - по X, 1 - по Y
   uint8_t method;           // Что измерять Topo=0,Phase=1, Ampl=2...
  uint16_t delayF;           // Задержка вперёд
  uint16_t delayB;           // Задержка назад
  uint16_t betweenPoints_x;  // Расстояние между точками по X дискрет
  uint16_t betweenPoints_y;  // Расстояние между точками по Y дискрет
   uint8_t sz;               // sz=1 Z; sz=2 Z,Signal add signal  
   uint8_t Ti;               // PID Gain 
  uint16_t diskretinstep;    // размер шага в дискрет
  uint16_t pause;            // ms? время ожидания в точке до начала измерения сигналов
   uint8_t flgOneFrame;      // need for Fast Scanning  =1 only one frame
};

                                       

struct ConfigCurrent
{
  uint16_t delayF;  // Задержка вперёд
  uint16_t delayB;  // Задержка назад
  uint8_t   Ti;     // PID Gain
};

class Scanner
{
public:

  Scanner();

  ~Scanner();

  void start_scan();

  void start_hoppingscan();
 
  void start_fastscan();

  void stop_scan();

  void update(const Config &config);

  void move_to(const Point &point, uint16_t delay);  // переместиться в нач.точку скана текущего скана

  void move_toX0Y0(int x, int y, int delay);  // //переместиться в начальную точку  скана из начальной точке предыдущего скана
  //add mf 
  Point  getX0Y0();
 
  void move_toZ0(int lid_name, int f, int p, int n, int dir);  // отвестись в безопастную начальную точку по Z

  void retract(); //втянуть сканер
 
  void protract(); //втянуть сканер
  
  void approacphm(const int16_t *const data); 

  void positioningXYZ(int lid_name, int f, int p, int n, int dir, int16_t gtmax,int16_t gtmin); // abs(n) 

  void start_frqscan(); //find resonance

  // void stopAll();
//
private:

  std::vector<uint16_t> vector_z, other_info;
  Point pos_,prev_point;
  Config  conf_;
};

#endif
