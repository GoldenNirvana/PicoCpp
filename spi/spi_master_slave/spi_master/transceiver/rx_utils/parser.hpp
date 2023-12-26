#ifndef Parser_h
#define Parser_h


#include <cstddef>
#include <cstdint>

class Parser
{
public:
  explicit Parser(int8_t *data, char newDiv = ',');

  ~Parser();

  void clear() const;

  int amount() const;

  int split();

  int16_t getInt(int num);

  float getFloat(int num);

  bool equals(int num, const char *comp);

  int parseInts(int32_t *data) const;

  char *buf = NULL;
  char **str = NULL;

  char *operator[](uint16_t idx) const;

  char div;
private:
};

#endif
