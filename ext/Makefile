#------------------------------------------------------------------------------
# Makefile to get and build external dependencies.

TOP = $(shell realpath ..)
DL_DIR = $(TOP)/dl
USR_DIR = $(TOP)/ext/usr

.PHONY: all
all: .stamp_sdk .stamp_extras .stamp_examples .stamp_gnu .stamp_pt .stamp_pio .stamp_usb

.PHONY: clean
clean:
	-rm -rf $(USR_DIR)
	-rm -rf $(SDK_DIR)
	-rm -rf $(PT_DIR)
	-rm -rf $(USB_DIR)
	-rm -rf $(EXTRAS_DIR)
	-rm -rf $(EXAMPLES_DIR)
	-rm .stamp*

#------------------------------------------------------------------------------
# pico sdk

SDK_VER = 2.0.0
SDK_URL = https://github.com/raspberrypi/pico-sdk/releases/download/$(SDK_VER)/pico-sdk-"$(SDK_VER).tar.gz
SDK_FILE = pico-sdk-$(SDK_VER).tar.gz
SDK_TGZ = $(DL_DIR)/$(SDK_FILE)
SDK_DIR = $(TOP)/ext/sdk

$(SDK_TGZ):
	mkdir -p $(DL_DIR)
	wget $(SDK_URL) -O $(SDK_TGZ)

.stamp_sdk: $(SDK_TGZ)
	mkdir -p $(SDK_DIR)
	tar -C $(SDK_DIR) -zxf $(SDK_TGZ) --strip-components 1
	touch $@


#------------------------------------------------------------------------------
# pico extras

EXTRAS_VER = 2.0.0
EXTRAS_URL = https://github.com/raspberrypi/pico-extras/archive/refs/tags/sdk-$(EXTRAS_VER).tar.gz
EXTRAS_FILE = pico-extras-$(EXTRAS_VER).tar.gz
EXTRAS_TGZ = $(DL_DIR)/$(EXTRAS_FILE)
EXTRAS_DIR = $(TOP)/ext/extras

$(EXTRAS_TGZ):
	mkdir -p $(DL_DIR)
	wget $(EXTRAS_URL) -O $(EXTRAS_TGZ)

.stamp_extras: $(EXTRAS_TGZ)
	mkdir -p $(EXTRAS_DIR)
	tar -C $(EXTRAS_DIR) -zxf $(EXTRAS_TGZ) --strip-components 1
	touch $@

#------------------------------------------------------------------------------
# pico examples

EXAMPLES_VER = 7e77a0c
EXAMPLES_URL = https://github.com/raspberrypi/pico-examples/tarball/$(EXAMPLES_VER)
EXAMPLES_FILE = pico-examples-$(EXAMPLES_VER).tar.gz
EXAMPLES_TGZ = $(DL_DIR)/$(EXAMPLES_FILE)
EXAMPLES_DIR = $(TOP)/ext/examples

$(EXAMPLES_TGZ):
	mkdir -p $(DL_DIR)
	wget $(EXAMPLES_URL) -O $(EXAMPLES_TGZ)

.stamp_examples: $(EXAMPLES_TGZ)
	mkdir -p $(EXAMPLES_DIR)
	tar -C $(EXAMPLES_DIR) -zxf $(EXAMPLES_TGZ) --strip-components 1
	touch $@

#------------------------------------------------------------------------------
# gnu tools

GNU_VER = 13.3.rel1
GNU_URL = https://developer.arm.com/-/media/Files/downloads/gnu/$(GNU_VER)/binrel/arm-gnu-toolchain-$(GNU_VER)-x86_64-arm-none-eabi.tar.xz
GNU_FILE = arm-gnu-toolchain-$(GNU_VER)-x86_64-arm-none-eabi.tar.xz
GNU_TXZ = $(DL_DIR)/$(GNU_FILE)

$(GNU_TXZ):
	mkdir -p $(DL_DIR)
	wget $(GNU_URL) -O $(GNU_TXZ)

.stamp_gnu: $(GNU_TXZ)
	mkdir -p $(USR_DIR)
	tar -C $(USR_DIR) -Jxf $(GNU_TXZ) --strip-components 1
	touch $@

#------------------------------------------------------------------------------
# picotool

PT_VER = 2.0.0
PT_URL = https://github.com/raspberrypi/picotool/releases/download/$(PT_VER)/picotool-$(PT_VER).tar.gz
PT_FILE = picotool-$(PT_VER).tar.gz
PT_TGZ = $(DL_DIR)/$(PT_FILE)
PT_DIR = $(TOP)/ext/picotool
PT_BUILD = $(PT_DIR)/build

$(PT_TGZ):
	mkdir -p $(DL_DIR)
	wget $(PT_URL) -O $(PT_TGZ)

.stamp_pt: $(PT_TGZ) .stamp_sdk
	mkdir -p $(PT_DIR)
	tar -C $(PT_DIR) -zxf $(PT_TGZ) --strip-components 1
	PICO_SDK_PATH=$(SDK_DIR) \
		cmake -GNinja \
		-DCMAKE_INSTALL_PREFIX=$(USR_DIR) \
		-DPICOTOOL_NO_LIBUSB=1 \
		-S $(PT_DIR) -B $(PT_BUILD)
	ninja -C $(PT_BUILD) install
	touch $@

#------------------------------------------------------------------------------
# tinyusb

USB_VER = 0.16.0
USB_URL = https://github.com/hathach/tinyusb/archive/refs/tags/$(USB_VER).tar.gz
USB_FILE = tinyusb-$(USB_VER).tar.gz
USB_TGZ = $(DL_DIR)/$(USB_FILE)
USB_DIR = $(TOP)/ext/tinyusb

$(USB_TGZ):
	mkdir -p $(DL_DIR)
	wget $(USB_URL) -O $(USB_TGZ)

.stamp_usb: $(USB_TGZ)
	mkdir -p $(USB_DIR)
	tar -C $(USB_DIR) -zxf $(USB_TGZ) --strip-components 1
	touch $@

#------------------------------------------------------------------------------
# pioasm

PIO_DIR = $(SDK_DIR)/tools/pioasm
PIO_BUILD = $(SDK_DIR)/build

.stamp_pio: .stamp_sdk
	cmake -GNinja \
		-DCMAKE_INSTALL_PREFIX=$(USR_DIR) \
		-S $(PIO_DIR) -B $(PIO_BUILD)
	ninja -C $(PIO_BUILD) install
	touch $@

#------------------------------------------------------------------------------
