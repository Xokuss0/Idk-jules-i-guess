# Debroforce Makefile
# Supports both PC (SDL2) and 3DS (libctru/citro2d) targets

TARGET_NAME := debroforce

# Directories
SOURCE_DIR := source
INCLUDE_DIR := include
BUILD_DIR := build

# Source files
CFILES := $(wildcard $(SOURCE_DIR)/*.c)

# PC Target setup
CFILES_PC := $(filter-out $(SOURCE_DIR)/hal_3ds.c, $(CFILES))
OFILES_PC := $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/pc/%.o, $(CFILES_PC))

CC_PC := gcc
CFLAGS_PC := -g -Wall -I$(INCLUDE_DIR)
LDFLAGS_PC := -lSDL2 -lSDL2_image -lm

.PHONY: all clean pc 3ds

all: pc

# PC Target
pc: $(BUILD_DIR)/pc $(TARGET_NAME)

$(TARGET_NAME): $(OFILES_PC)
	$(CC_PC) $(OFILES_PC) -o $@ $(LDFLAGS_PC)

$(BUILD_DIR)/pc/%.o: $(SOURCE_DIR)/%.c
	$(CC_PC) $(CFLAGS_PC) -c $< -o $@

$(BUILD_DIR)/pc:
	mkdir -p $@

# 3DS Target logic
ifneq ($(strip $(DEVKITARM)),)
ifneq ("$(wildcard $(DEVKITARM)/3ds_rules)","")
include $(DEVKITARM)/3ds_rules

# Architecture flags (Crucial for VFP register compatibility)
# -mfpu=vfp is required when -mfloat-abi=hard is used on ARMv6k
ARCH_3DS := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mfpu=vfp -mtp=soft

# Standard devkitPro paths
CTRULIB  := $(DEVKITPRO)/libctru
PORTLIBS := $(DEVKITPRO)/portlibs/3ds

# Filter sources for 3DS
CFILES_3DS := $(filter-out $(SOURCE_DIR)/hal_sdl.c, $(CFILES))
OFILES_3DS := $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/3ds/%.o, $(CFILES_3DS))

# 3DS specific flags
CFLAGS_3DS  := $(ARCH_3DS) -O2 -Wall -I$(INCLUDE_DIR) -I$(CTRULIB)/include -I$(PORTLIBS)/include -D__3DS__
LDFLAGS_3DS := -specs=3dsx.specs -g $(ARCH_3DS) -L$(CTRULIB)/lib -L$(PORTLIBS)/lib

3ds: $(BUILD_DIR)/3ds $(TARGET_NAME).3dsx

$(TARGET_NAME).3dsx: $(TARGET_NAME).elf

$(TARGET_NAME).elf: $(OFILES_3DS)
	$(CC) $(LDFLAGS_3DS) -o $@ $(OFILES_3DS) -lcitro2d -lcitro3d -lctru -lm

$(BUILD_DIR)/3ds/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS_3DS) -c $< -o $@

$(BUILD_DIR)/3ds:
	mkdir -p $@
else
3ds:
	@echo "Error: $(DEVKITARM)/3ds_rules not found. 3DS build requires a valid devkitPro installation."
	@exit 1
endif
else
3ds:
	@echo "Error: DEVKITARM is not set. 3DS build requires devkitPro environment."
	@exit 1
endif

clean:
	rm -rf $(BUILD_DIR) $(TARGET_NAME) $(TARGET_NAME).elf $(TARGET_NAME).3dsx $(TARGET_NAME).smdh
