#------------------------------------------------------------------------------
# Makefile to get and build external dependencies.

TOP = $(shell realpath ..)
DL_DIR = $(TOP)/dl
USR_DIR = $(TOP)/ext/usr

.PHONY: all
all: .stamp_sdk .stamp_extras .stamp_examples .stamp_playground .stamp_gnu_arm .stamp_gnu_riscv .stamp_pt .stamp_pio

.PHONY: clean
clean:
	-rm -rf $(USR_DIR)
	-rm -rf $(SDK_DIR)
	-rm -rf $(PT_DIR)
	-rm -rf $(EXTRAS_DIR)
	-rm -rf $(EXAMPLES_DIR)
	-rm -rf $(PLAYGROUND_DIR)
	-rm .stamp*

#------------------------------------------------------------------------------
# pico sdk

SDK_VER = 2.0.0
SDK_URL = https://github.com/raspberrypi/pico-sdk/releases/download/$(SDK_VER)/pico-sdk-$(SDK_VER).tar.gz
SDK_FILE = pico-sdk-$(SDK_VER).tar.gz
SDK_TGZ = $(DL_DIR)/$(SDK_FILE)
SDK_DIR = $(TOP)/ext/sdk

$(SDK_TGZ):
	mkdir -p $(DL_DIR)
	wget $(SDK_URL) -O $(SDK_TGZ)

.stamp_sdk: $(SDK_TGZ) .stamp_usb .stamp_tls .stamp_lwip .stamp_bt .stamp_cyw
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
# pico playground

PLAYGROUND_VER = 9930bb0
PLAYGROUND_URL = https://github.com/raspberrypi/pico-playground/tarball/$(PLAYGROUND_VER)
PLAYGROUND_FILE = pico-playground-$(PLAYGROUND_VER).tar.gz
PLAYGROUND_TGZ = $(DL_DIR)/$(PLAYGROUND_FILE)
PLAYGROUND_DIR = $(TOP)/ext/playground

$(PLAYGROUND_TGZ):
	mkdir -p $(DL_DIR)
	wget $(PLAYGROUND_URL) -O $(PLAYGROUND_TGZ)

.stamp_playground: $(PLAYGROUND_TGZ)
	mkdir -p $(PLAYGROUND_DIR)
	tar -C $(PLAYGROUND_DIR) -zxf $(PLAYGROUND_TGZ) --strip-components 1
	touch $@

#------------------------------------------------------------------------------
# gnu arm tools

GNU_ARM_VER = 13.3.rel1
GNU_ARM_URL = https://developer.arm.com/-/media/Files/downloads/gnu/$(GNU_ARM_VER)/binrel/arm-gnu-toolchain-$(GNU_ARM_VER)-x86_64-arm-none-eabi.tar.xz
GNU_ARM_FILE = arm-gnu-toolchain-$(GNU_ARM_VER)-x86_64-arm-none-eabi.tar.xz
GNU_ARM_TXZ = $(DL_DIR)/$(GNU_ARM_FILE)

$(GNU_ARM_TXZ):
	mkdir -p $(DL_DIR)
	wget $(GNU_ARM_URL) -O $(GNU_ARM_TXZ)

.stamp_gnu_arm: $(GNU_ARM_TXZ)
	mkdir -p $(USR_DIR)
	tar -C $(USR_DIR) -Jxf $(GNU_ARM_TXZ) --strip-components 1
	touch $@

#------------------------------------------------------------------------------
# gnu riscv tools

GNU_RISCV_VER = 20230622
GNU_RISCV_URL = https://buildbot.embecosm.com/job/corev-gcc-ubuntu2204/12/artifact/corev-openhw-gcc-ubuntu2204-$(GNU_RISCV_VER).tar.gz
GNU_RISCV_FILE = corev-openhw-gcc-ubuntu2204-$(GNU_RISCV_VER).tar.gz
GNU_RISCV_TGZ = $(DL_DIR)/$(GNU_RISCV_FILE)

$(GNU_RISCV_TGZ):
	mkdir -p $(DL_DIR)
	wget $(GNU_RISCV_URL) -O $(GNU_RISCV_TGZ)

.stamp_gnu_riscv: $(GNU_RISCV_TGZ)
	mkdir -p $(USR_DIR)
	tar -C $(USR_DIR) -zxf $(GNU_RISCV_TGZ) --strip-components 1
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
	cmake -GNinja \
		-DPICO_SDK_PATH=$(SDK_DIR) \
		-DCMAKE_INSTALL_PREFIX=$(USR_DIR) \
		-DPICOTOOL_NO_LIBUSB=1 \
		-S $(PT_DIR) -B $(PT_BUILD)
	ninja -C $(PT_BUILD) install
	touch $@

#------------------------------------------------------------------------------
# tinyusb (extract to sdk directory)

USB_VER = 4232642
USB_URL = https://github.com/hathach/tinyusb/tarball/$(USB_VER)
USB_FILE = tinyusb-$(USB_VER).tar.gz
USB_TGZ = $(DL_DIR)/$(USB_FILE)
USB_DIR = $(SDK_DIR)/lib/tinyusb

$(USB_TGZ):
	mkdir -p $(DL_DIR)
	wget $(USB_URL) -O $(USB_TGZ)

.stamp_usb: $(USB_TGZ)
	mkdir -p $(USB_DIR)
	tar -C $(USB_DIR) -zxf $(USB_TGZ) --strip-components 1
	touch $@

#------------------------------------------------------------------------------
# mbed tls (extract to sdk directory)

TLS_VER = 5a764e5
TLS_URL = https://github.com/Mbed-TLS/mbedtls/tarball/$(TLS_VER)
TLS_FILE = mbedtls-$(TLS_VER).tar.gz
TLS_TGZ = $(DL_DIR)/$(TLS_FILE)
TLS_DIR = $(SDK_DIR)/lib/mbedtls

$(TLS_TGZ):
	mkdir -p $(DL_DIR)
	wget $(TLS_URL) -O $(TLS_TGZ)

.stamp_tls: $(TLS_TGZ)
	mkdir -p $(TLS_DIR)
	tar -C $(TLS_DIR) -zxf $(TLS_TGZ) --strip-components 1
	touch $@

#------------------------------------------------------------------------------
# lwip (extract to sdk directory)

LWIP_VER = 0a0452b
LWIP_URL = https://github.com/lwip-tcpip/lwip/tarball/$(LWIP_VER)
LWIP_FILE = lwip-$(LWIP_VER).tar.gz
LWIP_TGZ = $(DL_DIR)/$(LWIP_FILE)
LWIP_DIR = $(SDK_DIR)/lib/lwip

$(LWIP_TGZ):
	mkdir -p $(DL_DIR)
	wget $(LWIP_URL) -O $(LWIP_TGZ)

.stamp_lwip: $(LWIP_TGZ)
	mkdir -p $(LWIP_DIR)
	tar -C $(LWIP_DIR) -zxf $(LWIP_TGZ) --strip-components 1
	touch $@

#------------------------------------------------------------------------------
# btstack (extract to sdk directory)

BT_VER = 2b49e57
BT_URL = https://github.com/bluekitchen/btstack/tarball/$(BT_VER)
BT_FILE = btstack-$(BT_VER).tar.gz
BT_TGZ = $(DL_DIR)/$(BT_FILE)
BT_DIR = $(SDK_DIR)/lib/btstack

$(BT_TGZ):
	mkdir -p $(DL_DIR)
	wget $(BT_URL) -O $(BT_TGZ)

.stamp_bt: $(BT_TGZ)
	mkdir -p $(BT_DIR)
	tar -C $(BT_DIR) -zxf $(BT_TGZ) --strip-components 1
	touch $@

#------------------------------------------------------------------------------
# cyw43 driver (extract to sdk directory)

CYW_VER = faf3638
CYW_URL = https://github.com/georgerobotics/cyw43-driver/tarball/$(CYW_VER)
CYW_FILE = cyw43-driver-$(CYW_VER).tar.gz
CYW_TGZ = $(DL_DIR)/$(CYW_FILE)
CYW_DIR = $(SDK_DIR)/lib/cyw43-driver

$(CYW_TGZ):
	mkdir -p $(DL_DIR)
	wget $(CYW_URL) -O $(CYW_TGZ)

.stamp_cyw: $(CYW_TGZ)
	mkdir -p $(CYW_DIR)
	tar -C $(CYW_DIR) -zxf $(CYW_TGZ) --strip-components 1
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
