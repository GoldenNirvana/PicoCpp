#include "parser.hpp"
#include <cstdlib>
#include <cstring>

Parser::Parser(int8_t *data, char newDiv)
{
  buf = (char *) data;
  div = newDiv;
}

Parser::~Parser()
{
  clear();
}

void Parser::clear() const
{
  if (str)
  { free(str); }
}

int Parser::amount() const
{
  int i = 0, count = 0;
  while (buf[i++])
  {
    if (buf[i] == div)
    {
      count++;
    }
  }
  return ++count;
}

int Parser::split()
{
  int am = amount();
  clear();
  str = (char **) malloc(am * sizeof(char *));
  str[0] = buf;
  int i = 0, j = 0;
  while (buf[i])
  {
    if (buf[i] == div)
    {
      buf[i] = '\0';
      str[++j] = buf + i + 1;
    }
    i++;
  }
  return am;
}

int Parser::parseInts(int32_t *data) const
{
  int count = 0;
  char *offset = buf;
  while (true)
  {
    data[count++] = atoi(offset);
    offset = strchr(offset, div);
    if (offset)
    {
      offset++;
    } else
    {
      break;
    }
  }
  return count;
}

char *Parser::operator[](uint16_t idx) const
{
  return str[idx];
}