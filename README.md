# picox

Raspberry Pi Pico Board Build Tools and Examples.

## Get the SDK and Tools

```
# make .stamp_ext
```



## Example Make File

```

TOP = $(shell realpath .)

# sdk tool paths
EXT = /home/jasonh/personal/picox/ext
SDK_PATH = $(EXT)/sdk
TOOLS_PATH = $(EXT)/usr/bin
PICOTOOL = $(EXT)/usr/lib/cmake/picotool
PIOASM = $(EXT)/usr/lib/cmake/pioasm

# pick a board
#BOARD = pico
#BOARD = pico2
BOARD = pimoroni_pico_plus2_rp2350

# pick a compiler
#COMPILER = pico_riscv_gcc
#COMPILER = pico_arm_cortex_m0plus_gcc
COMPILER = pico_arm_cortex_m33_gcc

# pick a platform
#PLATFORM = rp2040
#PLATFORM = rp2350-riscv
PLATFORM = rp2350-arm-s

BLD_PATH = $(TOP)/build
SRC_PATH = $(TOP)/src

.PHONY: all
all:
        cmake -GNinja -S $(SRC_PATH) -B $(BLD_PATH) \
                -DPICO_SDK_PATH=$(SDK_PATH) \
                -DPICO_TOOLCHAIN_PATH=$(TOOLS_PATH) \
                -Dpicotool_DIR=$(PICOTOOL) \
                -Dpioasm_DIR=$(PIOASM) \
                -DPICO_PLATFORM=$(PLATFORM) \
                -DPICO_COMPILER=$(COMPILER) \
                -DPICO_BOARD=$(BOARD)
        ninja -C $(BLD_PATH)

```


