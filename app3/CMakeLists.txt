cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(my_project C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

set(TARGET_NAME pio_blink)

add_executable(${TARGET_NAME}
    main.c
    ${COMMON_PATH}/util.c
    ${COMMON_PATH}/log.c
)

pico_generate_pio_header(${TARGET_NAME} ${CMAKE_CURRENT_LIST_DIR}/blinker.pio)

target_include_directories(${TARGET_NAME} PRIVATE
  ${COMMON_PATH}
)

target_compile_options(${TARGET_NAME} PRIVATE
  -Wall
  -Wextra
  -Wstrict-prototypes
  -Werror
)

target_link_libraries(${TARGET_NAME} PRIVATE
  pico_stdlib
  hardware_pio
)

pico_enable_stdio_uart(${TARGET_NAME} 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${TARGET_NAME})
