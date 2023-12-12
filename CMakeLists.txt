cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(sinestesia_pro)

pico_sdk_init()

add_executable(sinestesia_pro
        main.c
        ws2812.c
)


target_link_libraries(sinestesia_pro pico_stdlib hardware_adc hardware_gpio pico_multicore hardware_pio)

pico_enable_stdio_usb(sinestesia_pro 1)
pico_enable_stdio_uart(sinestesia_pro 0)

pico_add_extra_outputs(sinestesia_pro)
