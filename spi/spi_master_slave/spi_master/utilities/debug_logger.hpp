#ifndef PICO_EXAMPLES_DEBUG_LOGGER_HPP
#define PICO_EXAMPLES_DEBUG_LOGGER_HPP

#include <cstdint>
#include <string>
#include <iostream>

template<class T>
void log(T msg, int size, uint level_debug = 2)
{
  critical_section_enter_blocking(&criticalSection);
  if (level_debug <= DEBUG_LEVEL)
  {
    std::string out;
    for (int i = 0; i < size; ++i)
    {
      out += std::to_string(msg[i]) + " ";
    }
    out += '\n';
    uart_puts(uart1, out.data());
  }
  critical_section_exit(&criticalSection);
}

inline void log(std::string msg, uint level_debug = 2)
{
  critical_section_enter_blocking(&criticalSection);
  if (level_debug <= DEBUG_LEVEL)
  {
    uart_puts(uart1, msg.data());
  }
  critical_section_exit(&criticalSection);
}


#endif
