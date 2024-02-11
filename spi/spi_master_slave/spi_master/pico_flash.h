//.................................PICO_LOG DECLARATIONS................................

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <pico/time.h>
#include "hardware/flash.h"
#include "hardware/sync.h"

#ifndef PICOFLASH_H
#define PICOFLASH_H

#define FLASH_TARGET_OFFSET_0 (1792 * 1024)
#define FLASH_TARGET_OFFSET_1 (1792 * 1024 + 4096 * 1)
#define FLASH_TARGET_OFFSET_2 (1792 * 1024 + 4096 * 2)
#define FLASH_TARGET_OFFSET_3 (1792 * 1024 + 4096 * 3)
#define FLASH_TARGET_OFFSET_4 (1792 * 1024 + 4096 * 4)
#define FLASH_TARGET_OFFSET_5 (1792 * 1024 + 4096 * 5)
#define FLASH_TARGET_OFFSET_6 (1792 * 1024 + 4096 * 6)
#define FLASH_TARGET_OFFSET_7 (1792 * 1024 + 4096 * 7)
#define FLASH_TARGET_OFFSET_8 (1792 * 1024 + 4096 * 8)
#define FLASH_TARGET_OFFSET_9 (1792 * 1024 + 4096 * 9)


template<class T>
T pico_flash_read(long int FLASH_TARGET_OFFSET)
{
  T *flash_target_contents = (T *) (XIP_BASE + FLASH_TARGET_OFFSET);
  return *flash_target_contents;
}

template<class T>
int pico_flash_write(long int FLASH_TARGET_OFFSET, T *flash_data)
{
  uint32_t interrupts = save_and_disable_interrupts();
  flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
  flash_range_program(FLASH_TARGET_OFFSET, (uint8_t *) flash_data, FLASH_PAGE_SIZE);
  restore_interrupts(interrupts);
  return interrupts;
}

#endif // PICOFLASH_H
