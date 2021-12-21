# Author  -> alikadev
# Version -> 1.2
# Date    -> 09.01.07
# Desc    -> Build & run (qemu) AmthystOS



# var
RPI_VERSION ?= 3

BOOTMNT     ?= get-BOOTMNT

# build
ARMGNU      = aarch64-linux-gnu
FLAGS       = -DRPI_VERSION=$(RPI_VERSION) -Wall -nostdlib -nostartfiles \
		-ffreestanding -Iinc -mgeneral-regs-only


# dirs
BUILD_DIR   = build
BIN_DIR     = bin
DIST_DIR    = dist
INC_DIR     = inc
SRC_DIR     = src
BOOT_DIR    = bootloader
KERNEL_DIR  = kernel
OS_DIR      = os



# files
LINKER             = $(SRC_DIR)/linker.ld

BOOT_ASM_FILES     = $(addprefix $(BOOT_DIR)/,$(notdir $(wildcard $(SRC_DIR)/$(BOOT_DIR)/*.c)))
BOOT_ASM_FILES    += $(addprefix $(BOOT_DIR)/,$(notdir $(wildcard $(SRC_DIR)/$(BOOT_DIR)/*.S)))

KERNEL_ASM_FILES   = $(addprefix $(KERNEL_DIR)/,$(notdir $(wildcard $(SRC_DIR)/$(KERNEL_DIR)/*.c)))
KERNEL_ASM_FILES  += $(addprefix $(KERNEL_DIR)/,$(notdir $(wildcard $(SRC_DIR)/$(KERNEL_DIR)/*.S)))

OS_ASM_FILES       = $(addprefix $(OS_DIR)/,$(notdir $(wildcard $(SRC_DIR)/$(OS_DIR)/*.c)))
OS_ASM_FILES      += $(addprefix $(OS_DIR)/,$(notdir $(wildcard $(SRC_DIR)/$(OS_DIR)/*.S)))

SRC_BOOT_ASM       = $(addprefix $(SRC_DIR)/,$(BOOT_ASM_FILES))
SRC_KERNEL_ASM   = $(addprefix $(SRC_DIR)/,$(KERNEL_ASM_FILES))
SRC_OS_ARM       = $(addprefix $(SRC_DIR)/,$(OS_ASM_FILES))

OBJ_BOOT           = $(BOOT_ASM_FILES:$(SRC_DIR)/$(BOOT_DIR)/%.S=$(BUILD_DIR)/$(BOOT_DIR)/%_s.o)
OBJ_KERNEL         = $(KERNEL_ASM_FILES:$(SRC_DIR)/$(KERNEL_DIR)/%.S=$(BUILD_DIR)/$(KERNEL_DIR)/%_s.o)
OBJ_OS             = $(OS_ASM_FILES:$(SRC_DIR)/$(OS_DIR)/%.S=$(BUILD_DIR)/$(OS_DIR)/%_s.o)

OBJS               = $(shell find $(BUILD_DIR) -name '*.o')
DEP_FILES = $(OBJS:%.o=%.d)
-include $(DEP_FILES)


# out
OUT_IMAGE   = AmethystOS.img



.PHONY: all help clean always buildboot buildkernel buildos linkimg install build buildall installBuildTools run



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

	@echo "	buildall    > build and install install image"
	@echo "	build       > build all src and make image"
	@echo "	linkimg     > link obj in $(BUILD_DIR) for make $(BIN_DIR)/$(OUT_IMAGE)"
	@echo "	buildos     > build src from $(SRC_DIR)/$(OS_DIR) for make $(BUILD_DIR)/$(OS_DIR)"
	@echo "	buildkernel > build src from $(SRC_DIR)/$(KERNEL_DIR) for make $(BUILD_DIR)/$(KERNEL_DIR)"
	@echo "	buildboot   > build src from $(SRC_DIR)/$(BOOT_DIR) for make $(BUILD_DIR)/$(BOOT_DIR)"
	@echo "	install    > build BOOTMNT with image"
	@echo "	# installBuildTools > install used buildtools"
	@echo ""
	@echo "	run         > run in qemu (qemu-system-aarch64)"



installBuildTools:
	sudo apt-get update
	sudo apt-get install tree make gcc-arm-linux-gnueabi binutils-arm-linux-gnueabi qemu-system gcc-aarch64-linux-gnu



# ========== BUILD
# build bootloader
buildboot: buildbootWrite $(OBJ_BOOT) buildbootEnd
buildbootWrite:
	@echo
	@echo " --- buildboot ---"
	@echo "from: $(SRC_DIR)/$(BOOT_DIR)/"
	@echo "   src:  $(SRC_BOOT_ASM)"
	@echo
	@echo "to:   $(BUILD_DIR)/$(BOOT_DIR)"
	@echo "   obj:  $(OBJ_BOOT)"
	@echo

buildbootEnd:
	@echo "_____________________"
	@echo

$(BOOT_DIR)/%.S:
	@mkdir -p $(BUILD_DIR)/$(@D)
	@$(ARMGNU)-gcc $(FLAGS) -MMD -c $(SRC_DIR)/$@ -o $(BUILD_DIR)/$@.o
$(BOOT_DIR)/%.c:
	@mkdir -p $(BUILD_DIR)/$(@D)
	@$(ARMGNU)-gcc $(FLAGS) -MMD -c $(SRC_DIR)/$@ -o $(BUILD_DIR)/$@.o



# build kernel
buildkernel: buildkernelWrite $(OBJ_KERNEL) buildkernelEnd
buildkernelWrite:
	@echo
	@echo " --- buildkernel ---"
	@echo "from: $(SRC_DIR)/$(KERNEL_DIR)/"
	@echo "   src:  $(SRC_KERNEL_ASM)"
	@echo
	@echo "to:   $(BUILD_DIR)/$(KERNEL_DIR)"
	@echo "   obj:  $(OBJ_KERNEL)"
	@echo

buildkernelEnd:
	@echo "_____________________"
	@echo

$(KERNEL_DIR)/%.S:
	@mkdir -p $(BUILD_DIR)/$(@D)
	@$(ARMGNU)-gcc $(FLAGS) -MMD -c $(SRC_DIR)/$@ -o $(BUILD_DIR)/$@.o
$(KERNEL_DIR)/%.c:
	@mkdir -p $(BUILD_DIR)/$(@D)
	@$(ARMGNU)-gcc $(FLAGS) -MMD -c $(SRC_DIR)/$@ -o $(BUILD_DIR)/$@.o



# build OS
buildos: buildosWrite $(OBJ_OS) buildosEnd
buildosWrite:
	@echo
	@echo " --- buildos ---"
	@echo "from: $(SRC_DIR)/$(OS_DIR)/"
	@echo "   src:  $(SRC_OS_ARM)"
	@echo
	@echo "to:   $(BUILD_DIR)/$(OS_DIR)"
	@echo "   obj:  $(OBJ_OS)"
	@echo

buildosEnd:
	@echo "_____________________"
	@echo

$(OS_DIR)/%.S:
	@mkdir -p $(BUILD_DIR)/$(@D)
	@$(ARMGNU)-gcc $(FLAGS) -MMD -c $(SRC_DIR)/$@ -o $(BUILD_DIR)/$@.o
$(OS_DIR)/%.c:
	@mkdir -p $(BUILD_DIR)/$(@D)
	@$(ARMGNU)-gcc $(FLAGS) -MMD -c $(SRC_DIR)/$@ -o $(BUILD_DIR)/$@.o


# build armstub
armstub: armstubWrite armstub/build/armstub.S.o armstubEnd
	@$(ARMGNU)-ld --section-start=.text=0 -o armstub/build/armstub.elf armstub/build/armstub.S.o
	@$(ARMGNU)-objcopy armstub/build/armstub.elf -O binary $(BUILD_DIR)/armstub-new.bin
	@cp -f $(BUILD_DIR)/armstub-new.bin $(BOOTMNT)/
	@sync

armstubWrite:
	@echo
	@echo " --- armstub ---"

armstubEnd:
	@echo "_____________________"
	@echo


armstub/build/armstub.S.o: armstub/src/armstub.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(FLAGS) -MMD -c $< -o $@


# link objs
linkimg: linkimgWrite linkimgWrk linkimgEnd
linkimgWrite:
	@echo
	@echo " --- linkimg ---"
	@echo "Building for RPI $(value RPI_VERSION)"
	@echo
	@echo "from: $(BUILD_DIR)/"
	@echo "   $(OBJS)"
	@echo "to:   $(BIN_DIR)/$(OUT_IMAGE)"

linkimgWrk:
	@$(ARMGNU)-ld -T $(LINKER) -o $(BUILD_DIR)/$(OUT_IMAGE).elf $(OBJS)
	@$(ARMGNU)-objcopy $(BUILD_DIR)/$(OUT_IMAGE).elf -O binary $(BIN_DIR)/$(OUT_IMAGE)

linkimgEnd:
	@echo "_____________________"
	@echo



# install
install: installWrite installWrk installEnd
installWrite:
	@echo
	@echo " --- install ---"
	@echo "installing $(BIN_DIR)/$(OUT_IMAGE)"

installWrk:
ifeq ($(RPI_VERSION), 4)
	@cp $(BIN_DIR)/$(OUT_IMAGE) $(BOOTMNT)/kernel8-rpi4.img
	@echo "in to      $(BOOTMNT)"/kernel8-rpi4.img
else
	@cp $(BIN_DIR)/$(OUT_IMAGE) $(BOOTMNT)/kernel8.img
	@echo "in to      $(BOOTMNT)"/kernel8.img
endif
	@cp config.txt $(BOOTMNT)/
	@sync
	
installEnd:
	@echo "_____________________"
	@echo



# clean all and make OS
build: minimal clean always buildboot buildkernel buildos linkimg armstub
buildall: build install



# ========== MINIMAL
# ----- run
# run the img with qemu-system-aarch64
run:
	@qemu-system-aarch64 -M raspi3 -serial stdio -kernel $(BIN_DIR)/$(OUT_IMAGE)




# ========== RUN
# ----- clean
# clean build and bin dirs
clean:
	@echo
	@echo " --- clean ---"
	@echo "Cleaning build dir..."

	@rm ./$(BUILD_DIR)/* -rf
	@echo "Cleaning bin dir"
	@rm ./$(BIN_DIR)/* -rf

	@echo "_____________________"



# ----- always
# make dirs if not exist
minimal:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR) $(INC_DIR) $(DIST_DIR)

always:
	@echo
	@echo " --- always ---"

	@echo "Making dirs '$(SRC_DIR)/[$(BOOT_DIR) $(KERNEL_DIR) $(OS_DIR)]'"
	@mkdir -p $(SRC_DIR)/$(BOOT_DIR)  $(SRC_DIR)/$(KERNEL_DIR)  $(SRC_DIR)/$(OS_DIR)

	@echo "Making dirs '$(BUILD_DIR)/[$(BOOT_DIR) $(KERNEL_DIR) $(OS_DIR)]'"
	@mkdir -p $(BUILD_DIR)/$(BOOT_DIR)  $(BUILD_DIR)/$(KERNEL_DIR)  $(BUILD_DIR)/$(OS_DIR)

	@echo "_____________________"
