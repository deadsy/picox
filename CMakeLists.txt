cmake_minimum_required(VERSION 3.13)

# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

project(picox_project C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(picox)

add_subdirectory(src)

target_link_libraries(picox
  PRIVATE
    pico_stdlib
    pico_multicore
)

pico_enable_stdio_uart(picox 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(picox)
