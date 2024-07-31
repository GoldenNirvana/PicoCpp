cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

# Add blink example


# Add hello world example


add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Hardware-specific examples in subdirectories:
add_executable(uart_uart uart.cpp)
# Pull in basic dependencies
target_link_libraries(uart_uart pico_stdlib hardware_uart)
#target_link_libraries(spi_master pico_stdlib hardware_spi)

pico_enable_stdio_usb(spi_master 1)
pico_enable_stdio_uart(spi_master 1) 

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Werror")
#add_compile_options(-Werror)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")

# create map/bin/hex file etc.
pico_add_extra_outputs(uart_uart)
pico_add_uf2_output(uart_uart)




