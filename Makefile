
TOP = $(shell realpath .)

EXT = $(TOP)/ext

SDK_PATH = $(EXT)/sdk
EXTRAS_PATH = $(EXT)/extras
TOOLS_PATH = $(EXT)/usr/bin
PICOTOOL = $(EXT)/usr/lib/cmake/picotool
PIOASM = $(EXT)/usr/lib/cmake/pioasm

BLD_PATH = $(TOP)/build

SRC_PATH = $(TOP)/app0
#SRC_PATH = $(TOP)/app1

# pick a board
#BOARD = pico
#BOARD = pico2
#BOARD = vgaboard
BOARD = pimoroni_pico_plus2_rp2350

# pick a compiler
#COMPILER = pico_riscv_gcc
#COMPILER = pico_arm_cortex_m0plus_gcc
COMPILER = pico_arm_cortex_m33_gcc

# pick a platform
#PLATFORM = rp2040
#PLATFORM = rp2350-riscv
PLATFORM = rp2350-arm-s

.PHONY: all
all: .stamp_ext
	cmake -GNinja -S $(SRC_PATH) -B $(BLD_PATH) \
		-DPICO_SDK_PATH=$(SDK_PATH) \
		-DPICO_EXTRAS_PATH=$(EXTRAS_PATH) \
		-DPICO_TOOLCHAIN_PATH=$(TOOLS_PATH) \
		-Dpicotool_DIR=$(PICOTOOL) \
		-Dpioasm_DIR=$(PIOASM) \
		-DPICO_PLATFORM=$(PLATFORM) \
		-DPICO_COMPILER=$(COMPILER) \
		-DPICO_BOARD=$(BOARD)
	ninja -C $(BLD_PATH)

.PHONY: clean
clean:
	-rm -rf $(BLD_PATH)

.PHONY: clobber
clobber: clean
	make -C $(EXT) clean
	-rm .stamp_ext

.stamp_ext:
	make -C $(EXT) all
	touch $@
