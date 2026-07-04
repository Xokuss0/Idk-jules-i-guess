# Debroforce Makefile
# Supports both PC (SDL2) and 3DS targets

TARGET_NAME := debroforce

# Directories
SOURCE_DIR := source
INCLUDE_DIR := include
BUILD_DIR := build

# Source files
CFILES := $(wildcard $(SOURCE_DIR)/*.c)
OFILES := $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(CFILES))

# Compiler and Flags
CC := gcc
CFLAGS := -g -Wall -I$(INCLUDE_DIR)
LDFLAGS := -lSDL2 -lSDL2_image

# 3DS specific setup (if DEVKITARM is set)
ifneq ($(strip $(DEVKITARM)),)
    # This block is for when we have the 3DS toolchain ready
    # For now, we mainly focus on the cross-platform structure
endif

.PHONY: all clean pc

all: pc

pc: $(BUILD_DIR) $(TARGET_NAME)

$(TARGET_NAME): $(OFILES)
	$(CC) $(OFILES) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET_NAME)
