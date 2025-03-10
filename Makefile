# XScrabble v3 Makefile for macOS with XQuartz
#
# Provides build, test, installation and deployment targets

# Configuration
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
LOCAL_INSTALL_DIR = $(HOME)/.local

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE = $(BIN_DIR)/xscrabble

# Version info
VERSION = 3.0.0
BUILD_DATE = $(shell date +%Y%m%d)

# Default target (show help)
.PHONY: default
default: help ## Default target (shows help)

# Help target
.PHONY: help
help: ## Show available targets with descriptions
	@echo "XScrabble v3 ($(VERSION)) - Build System"
	@echo ""
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | sed 's/:[^:]*##/: ##/' | column -t -s '##'
	@echo ""

# Build aliases
.PHONY: all build
all: directories $(EXECUTABLE) ## Build the XScrabble executable
build: all ## Alias for 'all'

# Create necessary directories
.PHONY: directories
directories: ## Create build directories
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	@echo "Linking $(EXECUTABLE)..."
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build complete!"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
.PHONY: clean
clean: ## Remove build artifacts
	@echo "Cleaning build files..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete!"

# Debug build
.PHONY: debug
debug: CFLAGS += -g -O0 -DDEBUG ## Build with debug symbols
debug: clean all
	@echo "Debug build complete!"

# Run static analysis
.PHONY: analyze
analyze: ## Run static code analysis
	@echo "Running static analysis..."
	@clang --analyze $(INCLUDES) $(SOURCES) || echo "Analysis complete with warnings."

# Test targets
.PHONY: test test-board test-game test-dictionary
test: all ## Run all tests
	@echo "Running all tests..."
	@chmod +x $(TEST_DIR)/run_tests.sh
	@$(TEST_DIR)/run_tests.sh

test-board: all ## Run board component tests only
	@echo "Running board tests..."
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(TEST_DIR)/test_board $(TEST_DIR)/test_board.c $(SRC_DIR)/board.c $(LDFLAGS)
	@$(TEST_DIR)/test_board

test-game: all ## Run game logic tests only
	@echo "Running game logic tests..."
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(TEST_DIR)/test_game $(TEST_DIR)/test_game.c $(SRC_DIR)/game.c $(SRC_DIR)/board.c $(SRC_DIR)/dictionary.c $(LDFLAGS)
	@$(TEST_DIR)/test_game

test-dictionary: all ## Run dictionary tests only
	@echo "Running dictionary tests..."
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(TEST_DIR)/test_dictionary $(TEST_DIR)/test_dictionary.c $(SRC_DIR)/dictionary.c $(LDFLAGS)
	@$(TEST_DIR)/test_dictionary

# Build distribution package
.PHONY: dist
dist: all ## Create a distribution package
	@echo "Creating distribution package..."
	@mkdir -p dist/xscrabble
	@cp -r $(BIN_DIR) dist/xscrabble/
	@cp -r resources dist/xscrabble/
	@cp LICENSE dist/xscrabble/
	@cp README.org dist/xscrabble/
	@tar -czf dist/xscrabble-$(VERSION)-$(BUILD_DATE).tar.gz -C dist xscrabble
	@echo "Distribution package created: dist/xscrabble-$(VERSION)-$(BUILD_DATE).tar.gz"

# Installation targets
.PHONY: install install-local install-system uninstall uninstall-local

# Create necessary directories
$(LOCAL_INSTALL_DIR):
	@mkdir -p $@

$(LOCAL_INSTALL_DIR)/share/X11/app-defaults:
	@mkdir -p $@

# System installation (requires sudo)
install: all ## Install to system directories (requires sudo)
	@echo "Installing XScrabble to system directories..."
	@mkdir -p $(DESTDIR)/usr/local/bin
	@mkdir -p $(DESTDIR)/usr/local/share/xscrabble
	@cp $(EXECUTABLE) $(DESTDIR)/usr/local/bin/
	@cp -r resources/* $(DESTDIR)/usr/local/share/xscrabble/
	@chmod 755 $(DESTDIR)/usr/local/bin/xscrabble
	@echo "Installation complete. Run 'xscrabble' to start the game."

# Install app-defaults file for X11 (system-wide)
install-system: install ## Install X11 app-defaults to system directories
	@echo "Installing X11 resource files..."
	@cp resources/XScrabble $(DESTDIR)/usr/X11R6/lib/X11/app-defaults/ 2>/dev/null || \
	 cp resources/XScrabble $(DESTDIR)/opt/X11/lib/X11/app-defaults/ 2>/dev/null || \
	 cp resources/XScrabble $(DESTDIR)/usr/local/lib/X11/app-defaults/ 2>/dev/null || \
	 echo "Warning: Could not find X11 app-defaults directory"

# Local installation for testing
install-local: all $(LOCAL_INSTALL_DIR) $(LOCAL_INSTALL_DIR)/share/X11/app-defaults ## Install to ~/.local for testing
	@echo "Installing XScrabble to $(LOCAL_INSTALL_DIR)..."
	@mkdir -p $(LOCAL_INSTALL_DIR)/bin
	@mkdir -p $(LOCAL_INSTALL_DIR)/share/xscrabble
	@cp $(EXECUTABLE) $(LOCAL_INSTALL_DIR)/bin/
	@cp -r resources/* $(LOCAL_INSTALL_DIR)/share/xscrabble/
	@chmod 755 $(LOCAL_INSTALL_DIR)/bin/xscrabble
	@cp resources/XScrabble $(LOCAL_INSTALL_DIR)/share/X11/app-defaults/
	@echo "Installation complete."
	@echo "Add $(LOCAL_INSTALL_DIR)/bin to your PATH if it's not already there."
	@echo "Run 'export PATH=$(LOCAL_INSTALL_DIR)/bin:\$$PATH' to use immediately."
	@echo "Set XAPPLRESDIR=$(LOCAL_INSTALL_DIR)/share/X11/app-defaults"

# System uninstall
uninstall: ## Remove from system directories (requires sudo)
	@echo "Removing XScrabble from system directories..."
	@rm -f $(DESTDIR)/usr/local/bin/xscrabble
	@rm -rf $(DESTDIR)/usr/local/share/xscrabble
	@rm -f $(DESTDIR)/usr/X11R6/lib/X11/app-defaults/XScrabble 2>/dev/null || \
	 rm -f $(DESTDIR)/opt/X11/lib/X11/app-defaults/XScrabble 2>/dev/null || \
	 rm -f $(DESTDIR)/usr/local/lib/X11/app-defaults/XScrabble 2>/dev/null
	@echo "Uninstall complete."

# Local uninstall
uninstall-local: ## Remove from ~/.local
	@echo "Removing XScrabble from $(LOCAL_INSTALL_DIR)..."
	@rm -f $(LOCAL_INSTALL_DIR)/bin/xscrabble
	@rm -rf $(LOCAL_INSTALL_DIR)/share/xscrabble
	@rm -f $(LOCAL_INSTALL_DIR)/share/X11/app-defaults/XScrabble
	@echo "Local uninstall complete."

# XQuartz targets
.PHONY: xquartz run
xquartz: all ## Ensure XQuartz is running
	@echo "Setting up XQuartz..."
	@if ! pgrep -q XQuartz; then \
		echo "Starting XQuartz..."; \
		open -a XQuartz; \
		sleep 2; \
	else \
		echo "XQuartz is already running."; \
	fi

run: xquartz ## Run XScrabble with XQuartz
	@echo "Running XScrabble..."
	@DISPLAY=:0 $(EXECUTABLE)
