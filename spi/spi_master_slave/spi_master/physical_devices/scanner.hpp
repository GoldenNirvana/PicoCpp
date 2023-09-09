#ifndef PICO_EXAMPLES_SCANNER_HPP
#define PICO_EXAMPLES_SCANNER_HPP


#include <cstdint>
#include <vector>
#include "../utilities/Point.hpp"


struct Config
{
    uint32_t nPoints_x;  // Точек по линии X
    uint32_t nPoints_y;  // Точек по линии Y
    uint8_t path;  // 0 - по X, 1 - по Y
    uint8_t method;  //
    uint16_t delayF;  // Задержка вперёд
    uint16_t delayB;  // Задержка назад
    uint16_t betweenPoints_x;  // Расстояние между точками по X
    uint16_t betweenPoints_y;  // Расстояние между точками по Y
    uint8_t flag;  // Что измерять
};


class Scanner
{
public:

    Scanner();

    ~Scanner();

    void start_scan(const Point &point);

    void start_scan();

    void stop_scan();

    void update(const Config &config);

    void move_to(const Point &point, uint32_t delay);

private:

    std::vector<uint16_t> vector_z, other_info;
    Point pos_, prev_point;
    Config conf_;
};

Scanner scanner;


#endif
