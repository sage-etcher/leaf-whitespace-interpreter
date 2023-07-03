TARGET_EXEC := wsi

BUILD_DIR := ./build
SRC_DIR := ./src

SRC_FILES := $(SRC_DIR)/utils.c $(SRC_DIR)/ws_errors.c $(SRC_DIR)/ws_program.c $(SRC_DIR)/fileio.c $(SRC_DIR)/hashmap.c $(SRC_DIR)/wsi.c

INCLUDES := -I$(SRC_DIR)/include
LIBRARIES := -L$(SRC_DIR)/lib

BUILD_FLAGS := -std=c89 -pedantic
LINKER_FLAGS := $(INCLUDES) $(LIBRARIES)

CC := gcc

all:
	$(CC) -o $(BUILD_DIR)/$(TARGET_EXEC) $(BUILD_FLAGS) $(LINKER_FLAGS) $(SRC_FILES)