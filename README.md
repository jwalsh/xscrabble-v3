# XScrabble v3

A modern implementation of the classic Scrabble word game for X11/XQuartz on macOS and Unix systems.

## Features

- Clean, modern UI that maintains the classic gameplay
- Full dictionary support
- Customizable game settings
- Network play support (coming soon)
- Supports macOS with XQuartz and various Unix/Linux platforms

## Building from Source

### Prerequisites

- GCC or compatible C compiler
- X11 development libraries
- Make

### On macOS

1. Install XQuartz from https://www.xquartz.org/
2. Install required development tools:
   ```
   brew install gcc make
   ```
3. Build XScrabble:
   ```
   make
   ```

### On Linux/Unix

1. Install X11 development packages:
   ```
   # Debian/Ubuntu
   sudo apt-get install libx11-dev libxt-dev libxaw7-dev
   
   # Fedora/RHEL
   sudo dnf install libX11-devel libXt-devel libXaw-devel
   ```
2. Build XScrabble:
   ```
   make
   ```

## Usage

```
./bin/xscrabble
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by the original XScrabble by Vassilis Anagnostopoulos
- Modernized based on the FreeBSD port
