#include <iostream>
#include "loop/common_data/common_variables.hpp"
#include "loop/main_core.hpp"
#include "utilities/peripheral_functions.hpp"
#include "pico_flash.h"

// 224 * 8 * 1024
#define FLASH_TARGET_OFFSET (1792*1024)
#define FLASH_TARGET_OFFSET_SECOND (1792*1024)

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

int test_flash()
{
  uint32_t counter0 = 5;                                                                      //++ Declaration of Variables to be stored
  uint32_t counter1 = 105;
  uint32_t counter2 = 1005;

  uint32_t flash_data[256];
  //++ Declaring an array of size 256 to store the variables
  flash_data[0] = counter0;                                                                   //++ Storing variables on respective array indexes
  flash_data[1] = counter1;
  flash_data[2] = counter2;

  pico_flash_read(FLASH_TARGET_OFFSET, 3);                                                    //++ Flash operation to read 3 Flash Addresses before erasing and storing new data

  pico_flash_erase(FLASH_TARGET_OFFSET);                                                      //++ Flash operation to erase entire flash page ( 256 locations together )

  pico_flash_read(FLASH_TARGET_OFFSET, 3);                                                    //++ Flash operation to read 3 Flash Addresses after erasing entire page

  pico_flash_write(FLASH_TARGET_OFFSET, flash_data, 3);                                       //++ Flash operation to write the 3 Flash Address with the array containg the variables
}

int main()
{
  stdio_init_all();                                                                           //++ Initialize rp2040
  sleep_ms(10000);                                                                            //++ Wait for 15 secs

  std::cout << "\n System Starting..............\n";
  return test_flash();
//  return start_app();
}
