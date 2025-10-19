# Usage:
# make        # Compile all binaries
# make test   # Compile test binaries and run tests

# Compiler and Flags
CC := gcc
CFLAGS := -g -Wall -Wextra -Wpedantic -Wwrite-strings -Wswitch-enum -Wundef -Wformat=2 -Wstrict-overflow=5 -Isrc -Ibuild
LDFLAGS := -lm

# Directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

# Yacc and bison files
LEX_C := $(BUILD_DIR)/lex.yy.c
YACC_C := $(BUILD_DIR)/parser.tab.c
YACC_H := $(BUILD_DIR)/parser.tab.h
LEX_OBJ := $(BUILD_DIR)/lex.yy.o
YACC_OBJ := $(BUILD_DIR)/parser.tab.o

# Source files
LEX_SOURCES := $(SRC_DIR)/lambda.l
YACC_SOURCES := $(SRC_DIR)/lambda.y
C_SOURCES := $(wildcard $(SRC_DIR)/*.c) $(LEX_C) $(YACC_C)
C_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(C_SOURCES)))

# Executable
TARGET := $(BIN_DIR)/clam

# Run from make
.PHONY = all clean test

# Default target
all: $(TARGET)

# Create directories if not exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# AST depends on bison
$(BUILD_DIR)/ast.o: $(SRC_DIR)/ast.c $(BUILD_DIR)/parser.tab.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Generate Bison files (parser.tab.c, parser.tab.h)
$(YACC_C) $(YACC_H): $(YACC_SOURCES) | $(BUILD_DIR)
	bison -d -o $(YACC_C) $<

# Generate Flex file (lex.yy.c)
$(LEX_C): $(LEX_SOURCES) $(YACC_H) | $(BUILD_DIR)
	flex -o $(LEX_C) $(LEX_SOURCES)

# Compile Bison file (parser.tab.c, parser.tab.h)
$(YACC_OBJ): $(YACC_C)
	$(CC) $(CFLAGS) -c $(YACC_C) -o $(YACC_OBJ)
	ln -sf ../$(YACC_H) $(SRC_DIR)/parser.tab.h # simlink file for lsp

# Compile Flex file (lex.yy.c)
$(LEX_OBJ): $(LEX_C)
	$(CC) $(CFLAGS) -c $(LEX_C) -o $(LEX_OBJ)

# Link everything
$(TARGET): $(BIN_DIR) $(C_OBJS)
	$(CC) $(C_OBJS) $(LDFLAGS) -o $(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(SRC_DIR)/parser.tab.h

# Run test suite
test: $(TARGET)
	./test.sh $(TARGET)
