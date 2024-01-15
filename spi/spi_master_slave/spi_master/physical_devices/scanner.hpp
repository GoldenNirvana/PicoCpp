#ifndef PICO_EXAMPLES_SCANNER_HPP
#define PICO_EXAMPLES_SCANNER_HPP

#include <vector>
#include <string>
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
  std::vector<uint16_t> vector_z, other_info,vectorI_V,vectorA_Z;
  Point pos_, prev_point;
  Config conf_; 

  void stop_scan();                                   // возвращение сканера в  начальную точку скана
  
  void move_to(const Point &point, uint16_t delay);  // переместиться в начальную точку скана текущего скана

  void sendStrdata(std::string const& header, std::vector<int32_t> data);

public:

  Scanner();

  ~Scanner();

  void start_scan(int32_t *vector);

  void start_hopingscan(int32_t *vector);  //сканирование прыжками

  void start_fastscan(int32_t *vector);    // быстрое сканирование и вывод скана целиком, а не по линиям

  void approacphm(int32_t *vector);  // захват взаимодействия с контролем ворот

  void positioningXYZ(int32_t *vector); // abs(n) перемещение по  X,Y и Z (с контроллем ворот)

  void start_frqscan(); //поиск резонанса датчика

  void spectroscopyIV(int32_t *vector); // спектроскопия I-V

  void spectroscopyAZ(int32_t *vector); // спектроскопия Ampl-Z

  void scan_update(const Config &config);  // обновить параметры скaнирования

  void move_toX0Y0(int x, int y, int delay); //переместиться в начальную точку скана из начальной точке предыдущего скана

  void LID_move_toZ0(int lid_name, int f, int p, int n, int dir);// отвестись в безопасную начальную точку по Z при старте и выходе из программы
  
  void readADC();
  
  void scanner_retract_protract(int port, int flg);
  
  void retract();       // втянуть сканер

  void protract();      // втянуть сканер
 
  void LOOP_freeze_unfreeze(int port, int flg);  
 
  void freezeLOOP();    // заморизить ПИД

  void unfreezeLOOP();  // разморизить ПИД

  bool getHoppingFlg(); // получить флаг сканирования прыжками

  Point getX0Y0();      // получить текущую точку сканера в покое

};

#endif
