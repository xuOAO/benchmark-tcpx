# Compiler and flags
CC := gcc
CXX := g++
CFLAGS := -g -fPIC -Wall -Iinclude -I/usr/local/cuda/include
LDFLAGS := -g -shared -L/usr/local/cuda/lib64
LIBS := -Lthird-party/lib -lynl #-lcuda

# Directories
SRC_DIR := src
LIB_DIR := lib
OBJ_DIR := obj

# Output
TARGET := $(LIB_DIR)/libtcpx.so

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# Rule to build the shared library
$(TARGET): $(OBJ_DIR) $(LIB_DIR) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR);
	rm -rf $(LIB_DIR)/libtcpx.so

.PHONY: all clean
