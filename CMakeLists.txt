cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(sinestesia)

pico_sdk_init()

add_executable(sinestesia
        main.c
)


target_link_libraries(sinestesia pico_stdlib hardware_adc hardware_gpio ws2812)

pico_enable_stdio_usb(sinestesia 1)
pico_enable_stdio_uart(sinestesia 0)

pico_add_extra_outputs(sinestesia)
