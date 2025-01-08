# Compiler and flags
CC := gcc
CXX := g++
NVCC := gcc#nvcc
CFLAGS := -Wall -I../include -I/usr/local/cuda/include -g -O0 -rdynamic
NVFLAGS := -I../include -I/usr/local/cuda/include -g -O0
LDFLAGS := -L../lib -L/usr/local/cuda/lib64
LIBS := -ltcpx #-lcuda

# Directories and files
SRC_DIR := .
OBJ_DIR := ../obj
BIN_DIR := ../bin
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET := $(BIN_DIR)/ncdevmem-gpu

# Default target
all: $(TARGET)

# Create output binary
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	# $(NVCC) $(NVFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
	# $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	# $(NVCC) $(NVFLAGS) -cpp $< -o $@
	# $(CC) $(CFLAGS) -cpp $< -o $@
	$(CXX) $(CFLAGS) -cpp $< -o $@

# Clean up
.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR)/*.o $(TARGET)
