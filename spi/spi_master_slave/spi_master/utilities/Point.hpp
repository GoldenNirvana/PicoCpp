#ifndef PICO_EXAMPLES_POINT_HPP
#define PICO_EXAMPLES_POINT_HPP


#include <cstdint>

struct Point {
    uint16_t x;
    uint16_t y;

    bool operator==(const Point &other) const;

    bool operator!=(const Point &other) const;
};


#endif
