cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)


project(my_project C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(led_blink
    main.c
)

target_link_libraries(led_blink PRIVATE
  pico_stdlib
)

#pico_enable_stdio_usb(led_blink 1)
pico_enable_stdio_uart(led_blink 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(led_blink)
