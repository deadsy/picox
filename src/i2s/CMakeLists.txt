pico_generate_pio_header(picox ${CMAKE_CURRENT_LIST_DIR}/i2s.pio)

target_sources(picox
  PRIVATE
    ./i2s.c
)
