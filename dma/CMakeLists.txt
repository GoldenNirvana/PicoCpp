if (NOT PICO_NO_HARDWARE)
    add_subdirectory(channel_irq)
    add_subdirectory(control_blocks)
    add_subdirectory(hello_dma)
    add_subdirectory(sniff_crc)
endif ()
