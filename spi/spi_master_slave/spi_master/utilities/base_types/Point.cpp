#include "Point.hpp"

bool Point::operator==(const Point &other) const
{
  return (x == other.x && y == other.y);
}

bool Point::operator!=(const Point &other) const
{
  return !(*(this) == other);
}