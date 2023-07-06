add_executable(spi_master spi_master.cpp Parser.h Spi.cpp Spi.hpp Decoder.cpp Decoder.hpp Utilities.hpp peripheral_functions.hpp LinearDriver.cpp LinearDriver.hpp ad5664.cpp ad5664.hpp)

# Pull in basic dependencies
target_link_libraries(spi_master pico_stdlib hardware_spi hardware_gpio pico_multicore)
#target_link_libraries(spi_master pico_stdlib hardware_spi)

pico_enable_stdio_usb(spi_master 1)
pico_enable_stdio_uart(spi_master 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(spi_master)

# add url via pico_set_program_url
example_auto_set_url(spi_master)
