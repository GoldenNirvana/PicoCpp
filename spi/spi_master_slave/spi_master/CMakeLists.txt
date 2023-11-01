add_executable(spi_master spi_master.cpp transceiver/rx_utils/parser.hpp utilities/base_types/Spi.cpp
        utilities/base_types/Spi.hpp utilities/base_types/Decoder.cpp utilities/base_types/Decoder.hpp
        transceiver/rx_core.hpp transceiver/rx_core.cpp
        utilities/peripheral_functions.hpp utilities/peripheral_functions.cpp
        physical_devices/LinearDriver.cpp physical_devices/LinearDriver.hpp
        devices/ad5664.cpp devices/ad5664.hpp
        utilities/base_types/io_ports.cpp utilities/base_types/io_ports.h utilities/peripheral_functions.cpp
        utilities/base_types/Point.cpp utilities/base_types/Point.hpp physical_devices/scanner.cpp physical_devices/scanner.hpp
        transceiver/rx_core.cpp transceiver/rx_core.hpp
        loop/common_data/common_variables.hpp
        loop/common_data/common_variables.cpp transceiver/rx_utils/parser.cpp
        loop/main_core.cpp loop/main_core.hpp utilities/hardcoded_functions.hpp
        utilities/hardcoded_functions.cpp utilities/base_types/core_config.hpp
        utilities/base_types/core_config.hpp
        devices/DAC8563.cpp devices/DAC8563.hpp utilities/debug_logger.hpp utilities/debug_logger.cpp)
# Pull in basic dependencies
target_link_libraries(spi_master pico_stdlib hardware_spi hardware_gpio pico_multicore pico_mem_ops hardware_pio hardware_dma)
#target_link_libraries(spi_master pico_stdlib hardware_spi)

pico_enable_stdio_usb(spi_master 1)
pico_enable_stdio_uart(spi_master 0)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Werror")
#add_compile_options(-Werror)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")

# create map/bin/hex file etc.
pico_add_extra_outputs(spi_master)
pico_add_uf2_output(spi_master)

# add url via pico_set_program_url
example_auto_set_url(spi_master)
