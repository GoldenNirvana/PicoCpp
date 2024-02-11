#include <iostream>
#include "loop/common_data/common_variables.hpp"
#include "loop/main_core.hpp"
#include "utilities/peripheral_functions.hpp"
#include "pico_flash.h"

uint32_t DEBUG_LEVEL = 2;

int start_app()
{
  critical_section_init(&criticalSection);
  if (!critical_section_is_initialized(&criticalSection))
  {
    activateError();
  }
  dark();
  setDefaultSettings();
  MainCore mainCore;
  mainCore.loop();
  return 0;
}

struct Data_example
{
  int16_t a;
  int16_t b;
  int16_t c;
  int16_t d;
};


int test_flash()
{

  // Запись
  // Любая структура размером меньше 256 байт: sizeof(data) <= 256!!!
  Data_example data{};
  data.a = 8213;
  data.b = 551;
  data.c = 4;
  data.d = 7499;
  // При записи передать страницу (FLASH_TARGET_OFFSET_0 - FLASH_TARGET_OFFSET_9) на которую будет записана data
  // WARNING !!! После завершнения функции программа завершается. Ищу как поправить.
  pico_flash_write<Data_example>(FLASH_TARGET_OFFSET_8, &data);

  // Чтение
  // Считывание с указанной страницы
  data = pico_flash_read<Data_example>(FLASH_TARGET_OFFSET_8);
  std::cout << data.a << '\n';
  std::cout << data.b << '\n';
  std::cout << data.c << '\n';
  std::cout << data.d << '\n';

  std::cout << "\n All done..............\n";
  return 0;
}

int main()
{
  stdio_init_all();
  sleep_ms(3000);
  std::cout << "\n System Starting..............\n";
  return test_flash();
//  return start_app();
}
