#ifndef Parser_h
#define Parser_h


#include <cstddef>
#include <cstdint>

class Parser
{
public:
  explicit Parser(char *data, char newDiv = ',');

  ~Parser();

  void clear();

  int amount();

  int split();

  int16_t getInt(int num);

  float getFloat(int num);

  bool equals(int num, const char *comp);

  int parseInts(int32_t *data);

  char *buf = NULL;
  char **str = NULL;

  char *operator[](uint16_t idx);

  char div;
private:
};

#endif
