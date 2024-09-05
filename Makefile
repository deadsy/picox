
TOP = $(shell realpath .)

EXT = $(TOP)/ext

SDK_PATH = $(EXT)/sdk
BLD_PATH = $(TOP)/build
SRC_PATH = $(TOP)/src
TOOLS_PATH = $(EXT)/usr/bin
PICOTOOL_PATH = $(EXT)/usr/picotool

# pick a board ... (sdk names)
#BOARD = pico
#BOARD = pico2
BOARD = pimoroni_pico_plus2_rp2350

.PHONY: all
all: .stamp_ext
	PICO_SDK_PATH=$(SDK_PATH) \
		cmake -GNinja -S $(SRC_PATH) -B $(BLD_PATH) \
		-DPICO_TOOLCHAIN_PATH=$(TOOLS_PATH) \
		-Dpicotool_DIR=$(PICOTOOL_PATH) \
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
