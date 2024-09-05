cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(picox_project C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(picox)

add_subdirectory(src)

target_link_libraries(picox
  PRIVATE
    pico_stdlib
    pico_audio_i2s
    #pico_multicore
)

target_compile_definitions(picox PRIVATE
  # compile time configuration of I2S
  PICO_AUDIO_I2S_MONO_INPUT=1
)

pico_enable_stdio_uart(picox 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(picox)
