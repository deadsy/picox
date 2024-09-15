cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(my_project C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

set(TARGET_NAME audio_test)

add_executable(${TARGET_NAME}
    main.c
)

target_compile_options(${TARGET_NAME} PRIVATE
  -Wall
#  -Wextra
#  -Wstrict-prototypes
#  -Werror
)

target_link_libraries(${TARGET_NAME} PRIVATE
  pico_stdlib
  pico_audio_i2s
)

target_compile_definitions(${TARGET_NAME} PRIVATE
  # compile time configuration of I2S
  PICO_AUDIO_I2S_MONO_INPUT=1
  #define for our example code
  USE_AUDIO_I2S=1
)

pico_enable_stdio_uart(${TARGET_NAME} 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${TARGET_NAME})
