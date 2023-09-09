add_executable(spi_master spi_master.cpp Parser.h Spi.cpp
        Spi.hpp utilities/Decoder.cpp utilities/Decoder.hpp
        Utilities.hpp
        utilities/peripheral_functions.hpp utilities/peripheral_functions.cpp
        physical_devices/LinearDriver.cpp physical_devices/LinearDriver.hpp
        devices/ad5664.cpp devices/ad5664.hpp
        utilities/io_ports.cpp utilities/io_ports.h utilities/peripheral_functions.cpp utilities/Point.cpp utilities/Point.hpp physical_devices/scanner.cpp physical_devices/scanner.hpp)
# Pull in basic dependencies
target_link_libraries(spi_master pico_stdlib hardware_spi hardware_gpio pico_multicore pico_mem_ops hardware_pio hardware_dma)
#target_link_libraries(spi_master pico_stdlib hardware_spi)

pico_enable_stdio_usb(spi_master 1)
pico_enable_stdio_uart(spi_master 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(spi_master)

# add url via pico_set_program_url
example_auto_set_url(spi_master)
