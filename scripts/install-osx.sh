#!/bin/bash
# XScrabble v3 - macOS Installation Script

# Ensure XQuartz is installed
if ! command -v xquartz &> /dev/null; then
    echo "XQuartz not found. Installing..."
    brew install --cask xquartz
fi

# Create installation directories
sudo mkdir -p /usr/local/bin
sudo mkdir -p /usr/local/share/xscrabble

# Copy files
sudo cp bin/xscrabble /usr/local/bin/
sudo cp -r resources/* /usr/local/share/xscrabble/

# Set permissions
sudo chmod 755 /usr/local/bin/xscrabble

echo "XScrabble v3 installed successfully."
echo "Run 'xscrabble' to start the game."
