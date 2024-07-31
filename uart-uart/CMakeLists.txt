# Hardware-specific examples in subdirectories:
add_executable(uart_uart uart.cpp)
# Pull in basic dependencies
target_link_libraries(uart_uart pico_stdlib hardware_uart)
pico_enable_stdio_usb(spi_master 1)
pico_enable_stdio_uart(spi_master 1) 

# create map/bin/hex file etc.
pico_add_extra_outputs(uart_uart)
pico_add_uf2_output(uart_uart)




