#include "pico_flash.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "hardware/flash.h"
#include "hardware/sync.h"


uint32_t *pico_flash_read(long int FLASH_TARGET_OFFSET, size_t len)
{
  const uint32_t *flash_target_contents = (const uint32_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
  uint32_t *stored_value = (uint32_t *) calloc(len, sizeof(uint32_t));

  for (size_t i = 0; i < len; ++i)
  {
    std::cout << "STORED VALUE = " << flash_target_contents[i] << '\n';
    stored_value[i] = flash_target_contents[i];
  }

  return stored_value;                                                                                //++ Returns the pointer pointing to the array
}


int pico_flash_write(long int FLASH_TARGET_OFFSET, const uint32_t *flash_data, size_t num)
{
  uint32_t interrupts = save_and_disable_interrupts();
  flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);

  flash_range_program(FLASH_TARGET_OFFSET, reinterpret_cast<const uint8_t *>(flash_data), FLASH_PAGE_SIZE);
  restore_interrupts(interrupts);
}


void pico_flash_erase(long int FLASH_TARGET_OFFSET)
{
  int8_t stored_value = 0;
  uint32_t interrupts = save_and_disable_interrupts();
  flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);

  std::cout << "erase\n";

  flash_range_program(FLASH_TARGET_OFFSET, reinterpret_cast<const uint8_t *>(stored_value), FLASH_PAGE_SIZE);
  restore_interrupts(interrupts);
}
