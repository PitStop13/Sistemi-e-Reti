# Variables
CC = gcc
OBJ_DIR = obj
BIN_DIR = bin
TARGET = program

# Source files
SOURCES = $(wildcard *.c) $(wildcard *.h)
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)
PROGRAM_OBJECTS = $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS))

# Test source files
TEST_SOURCES = $(wildcard tests/*.c) $(wildcard tests/testing/*.c)
TEST_OBJECTS = $(TEST_SOURCES:%.c=$(OBJ_DIR)/%.o)

# Program Rules
run: $(BIN_DIR)/$(TARGET)
	@$(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@$(CC) -o $@ $^

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@$(CC) -c $< -o $@

# Test rules
test: $(BIN_DIR)/test_exec | $(BIN_DIR)
	@$(BIN_DIR)/test_exec

$(BIN_DIR)/test_exec: $(TEST_OBJECTS) $(PROGRAM_OBJECTS) | $(BIN_DIR)
	@$(CC) -I tests/testing -o $@ $^

# Create directories if they don't exist
$(BIN_DIR):
	@if [ ! -d "$(BIN_DIR)" ]; then mkdir -p $(BIN_DIR); fi

$(OBJ_DIR):
	@if [ ! -d "$(OBJ_DIR)" ]; then mkdir -p $(OBJ_DIR)/tests/testing; fi

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

run_test: test
	@$(BIN_DIR)/test_exec

.PHONY: all clean test run run_test $(BIN_DIR) $(OBJ_DIR)
