
target_compile_options(picox
  PRIVATE
    -Wall
    -Wextra
    -Wstrict-prototypes
)

target_compile_definitions(picox
  PRIVATE
    LOG_USE_COLOR
)

target_sources(picox
  PRIVATE
    ./main.c
    ./log.c
)

