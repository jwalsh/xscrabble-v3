# XScrabble Makefile for macOS with XQuartz

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -L/opt/X11/lib -lX11 -lXext -lXt -lXaw -lXmu
INCLUDES = -I/opt/X11/include -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = include
TEST_DIR = tests

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE = $(BIN_DIR)/xscrabble

# Phony targets
.PHONY: all clean test dist

# Default target
all: directories $(EXECUTABLE)

# Create necessary directories
directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run tests
test: all
	@echo "Running tests..."
	$(TEST_DIR)/run_tests.sh

# Build distribution package
dist: all
	@echo "Creating distribution package..."
	mkdir -p dist/xscrabble
	cp -r $(BIN_DIR) dist/xscrabble/
	cp -r resources dist/xscrabble/
	cp README.md LICENSE dist/xscrabble/
	tar -czf dist/xscrabble-`date +%Y%m%d`.tar.gz -C dist xscrabble
	@echo "Distribution package created: dist/xscrabble-`date +%Y%m%d`.tar.gz"

# Install
install: all
	@echo "Installing XScrabble..."
	mkdir -p $(DESTDIR)/usr/local/bin
	mkdir -p $(DESTDIR)/usr/local/share/xscrabble
	cp $(EXECUTABLE) $(DESTDIR)/usr/local/bin/
	cp -r resources/* $(DESTDIR)/usr/local/share/xscrabble/

# XQuartz specific targets
xquartz: all
	@echo "Setting up for XQuartz..."
	# Make sure XQuartz is running
	open -a XQuartz
	# Wait a moment for XQuartz to initialize
	sleep 1

# Run in XQuartz
run-xquartz: xquartz
	DISPLAY=:0 $(EXECUTABLE)
