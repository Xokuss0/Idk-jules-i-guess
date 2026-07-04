# Debroforce Makefile
# Supports both PC (SDL2) and 3DS (libctru/citro2d) targets

TARGET_NAME := debroforce

# Directories
SOURCE_DIR := source
INCLUDE_DIR := include
BUILD_DIR := build

# Source files
CFILES := $(wildcard $(SOURCE_DIR)/*.c)
OFILES_PC := $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/pc/%.o, $(CFILES))

# PC Flags
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
# Only attempt to include if the file actually exists
ifneq ("$(wildcard $(DEVKITARM)/3ds_rules)","")
include $(DEVKITARM)/3ds_rules

# Explicitly add libctru and portlibs paths to standard devkitARM flags
CTRULIB := $(DEVKITPRO)/libctru
PORTLIBS := $(DEVKITPRO)/portlibs/3ds
CFLAGS  += -I$(CTRULIB)/include -I$(PORTLIBS)/include
LDFLAGS += -L$(CTRULIB)/lib -L$(PORTLIBS)/lib

OFILES_3DS := $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/3ds/%.o, $(CFILES))

3ds: $(BUILD_DIR)/3ds $(TARGET_NAME).3dsx

$(TARGET_NAME).3dsx: $(TARGET_NAME).elf

$(TARGET_NAME).elf: $(OFILES_3DS)
	$(CC) $(LDFLAGS) -o $@ $(OFILES_3DS) -lcitro2d -lcitro3d -lctru -lm

$(BUILD_DIR)/3ds/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -D__3DS__ -I$(INCLUDE_DIR) -c $< -o $@

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
