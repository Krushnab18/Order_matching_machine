# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Directory structure
SRC_DIR = src/main
INCLUDE_DIR = src/include
OBJ_DIR = obj

# Collect all source files and define object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_MAIN = $(OBJ_DIR)/main.o

# Target to build
TARGET = sim

# Default make target
all: $(TARGET)

# Link everything into the target
$(TARGET): $(OBJ_FILES) $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $^

# Compile the source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Compile the main.c file
$(OBJ_DIR)/main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# Rebuild from scratch
rebuild: clean all

.PHONY: all clean rebuild
