add_executable(hello_serial
        hello_serial.c
        )

# pull in common dependencies
target_link_libraries(hello_serial pico_stdlib pico_stdlib hardware_spi hardware_gpio pico_multicore pico_mem_ops hardware_pio hardware_dma)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_serial)

# add url via pico_set_program_url
example_auto_set_url(hello_serial)
