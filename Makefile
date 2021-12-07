# Author  -> alikadev
# Version -> 0.1
# Date    -> 21.12.07
# Desc    -> Build & run (qemu) AmthystOS



# var
RPI_VERSION ?= 3



# build
ARMGNU      = aarch64-linux-gnu
FLAGS       = -DRPI_VERSION=$(RPI_VERSION) -Wall -nostdlib -nostartfiles \
		-ffreestanding -Iinc -mgeneral-regs-only


# dirs
BUILD_DIR   = build
BIN_DIR     = bin
DIST_DIR    = dist
SRC_DIR     = src
BOOT_DIR    = bootloader
KERNEL_DIR  = kernel
OS_DIR      = os



# out
OUT_IMAGE   = AmethystOS.img



.PHONY: all help clean always buildboot buildkernel buildos buildimg buildall installBuildTools



# ========== HELP
# ----- help / all
# print all commandes
all: help
help:
	@echo "Welcome in AmethystOS Makefile"
	@echo "Please use these args for use-it"

	@echo "	help / all  > print this"

	@echo "	clean       > clean build & bin dirs"
	@echo "	always      > make dirs for build"

	@echo "	buildall    > build all scr and link it"
	@echo "	buildimg    > link obj in $(BUILD_DIR) for make $(BIN_DIR)/$(OUT_IMG)"
	@echo "	buildos     > build src from $(SRC_DIR)/$(OS_DIR) for make $(BUILD_DIR)/$(OS_DIR)"
	@echo "	buildkernel > build src from $(SRC_DIR)/$(KERNEL_DIR) for make $(BUILD_DIR)/$(KERNEL_DIR)"
	@echo "	buildboot   > build src from $(SRC_DIR)/$(BOOT_DIR) for make $(BUILD_DIR)/$(BOOT_DIR)"
	@echo "	# installBuildTools > install used buildtools"
	@echo ""
	@echo "	run         > run in qemu (qemu-system-aarch64)"



installBuildTools:
	sudo apt-get install tree make gcc-arm-linux-gnueabi binutils-arm-linux-gnueabi



# ========== BUILD
# build bootloader
buildboot:
	@echo "Not implemented..."



# build kernel
buildkernel:
	@echo "Not implemented..."



# build OS
buildos:
	@echo "Not implemented..."



# link objs
buildimg:
	@echo "Not implemented..."



# clean all and make OS
buildall:
	@echo "Not implemented..."



# ========== MINIMAL
# ----- clean
# clean build and bin dirs
clean:
	@echo "Cleaning build dir..."
	@rm ./$(BUILD_DIR)/* -rf
	@echo "Cleaning bin dir"
	@rm ./$(BIN_DIR)/* -rf



# ----- always
# make dirs if not exist
always:
	@echo "Make '$(BIN_DIR) inc $(DIST_DIR)' dirs"
	@mkdir -p $(BUILD_DIR) $(BIN_DIR) inc $(DIST_DIR)
	@echo "Make '$(SRC_DIR)/[$(BOOT_DIR) $(KERNEL_DIR) $(OS_DIR)]'"
	@mkdir -p $(SRC_DIR)/$(BOOT_DIR)  $(SRC_DIR)/$(KERNEL_DIR)  $(SRC_DIR)/$(OS_DIR)
	@mkdir -p $(BUILD_DIR)/$(BOOT_DIR)  $(BUILD_DIR)/$(KERNEL_DIR)  $(BUILD_DIR)/$(OS_DIR)
