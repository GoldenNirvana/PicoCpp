#include "parser.hpp"
#include <cstdlib>
#include <cstring>
#include <vector>
#include "../../loop/common_data/common_variables.hpp"



Parser::Parser(char *data, char newDiv)
{
  buf = data;
  div = newDiv;
}

Parser::~Parser()
{
  clear();
}

void Parser::clear()
{
  if (str)
  { free(str); }
}

int Parser::amount()
{
  int i = 0, count = 0;
  while (buf[i++])
  {
    if (buf[i] == div)
    { count++; }
  }  // подсчёт разделителей
  return ++count;
}

int Parser::split()
{
  int am = amount();            // количество данных
  clear();                      // освобождаем буфер
  str = (char **) malloc(am * sizeof(char *)); // создаём буфер
  str[0] = buf;                 // строка 0
  int i = 0, j = 0;             // счётчики
  while (buf[i])
  {              // пока не NULL
    if (buf[i] == div)
    {        // если разделитель
      buf[i] = '\0';            // меняем на NULL
      str[++j] = buf + i + 1;   // запоминаем начало строки
    }
    i++;
  }
  return am;
}

int16_t Parser::getInt(int num)
{
  return atol(str[num]);
}

float Parser::getFloat(int num)
{
  return atof(str[num]);
}

bool Parser::equals(int num, const char *comp)
{
  return !strcmp(str[num], comp);
}
int32_t Parser::parseInts(std::vector<int32_t> &data)
{
  char *offset = buf;  
  data.clear(); 
  while (true)
  {
    data.emplace_back((int32_t)atoi(offset)); 
    offset = strchr(offset, div);
    if (offset)   { offset++; }
    else          { break;    }
  }
  return data.size();
}
int32_t Parser::parseInts(std::vector<int32_t> &data,std::vector<int32_t> &uparams)
{
  int count = 0;
  bool flgUpdate;
  char *offset = buf;
  while (true)
  {
    if (count==0)
    {
      flgUpdate=bool((int32_t)atoi(offset)==CONFIG_UPDATECmd);
      if (flgUpdate) uparams.clear();
      //else 
      data.clear();
      count++;
    }
    if (flgUpdate) uparams.emplace_back((int32_t)atoi(offset)); 
    else           data.emplace_back((int32_t)atoi(offset)); 
    offset = strchr(offset, div);
    if (offset)   { offset++; }
    else          { break;    }
  }
  return data.size();
}

char *Parser::operator[](uint16_t idx)
{
  return str[idx];
}