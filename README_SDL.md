Gamebuino META / SDL Wrapper
============================

## What is this?

*This is not an emulator.*  This project attempts to makes minor changes to the standard Gambuino META library to allow it to compile natively against [libSDL](https://www.libsdl.org) rather than compiling for an actual hardward device.  The resulting binary will simply be an executable you can run directly.

### Why might you want to do that?

To design/play test games without having an actual META device.

### What is supported?

- LCD hardware emulation 160x128, 12/16/18 bit modes
- Buttons via the keyboard (cursor keys, A, Z (B), M (Menu), Return (Home))
- The SDFat emulation layer (FakeSD) may not be fully implimented. (I implimented enough to get most of my test sketches working.)
- Sound works. (It just passes SOUND_FREQ directly to SDL, though I've only tested 44.1KHz.)
- The filesystem found locally in `fs` will be the contents of the SD card.
- Some effort is made to "root" the sketch into the `fs` folder but since you're running this code locally on your own system please use caution and know what you are compiling (always a good idea).

## How do I get started?

First you'll need to build the library itself.

### Mac / Linux

```
git clone https://github.com/yyyc514/Gamebuino-Meta.git
cd Gamebuino-Meta
# checkout the SDL branch
git checkout sdl
cd src
make
```

Next build your application:

First, get a base Makefile for your game.
Example: https://github.com/yyyc514/meta-solitaire/blob/master/meta-solitaire/Makefile

```
cd mygame
make
mkdir fs
./mygame
```

Because the library builds the sketch name into the library itself (sad face) if you're building and testing multiple games you should really `make clean` the library every time you switch to building a different application.  Either that or just have a separate copy of the library for each sketch.

I haven't tested this on Linux.  The `Makefile` will likely need alteration.

## Requirements

This may not be a complete list.  I already have so many developer tools I'm not sure what I might have just missed because it was already part of my environment.

### On the Mac

- XCode
- XCode Command Line tools
- SDL2 (`brew install sdl2`)
- [Homebrew]()https://brew.sh (if you're giong to install SDL2 via Homebrew)
- Arduino IDE (invidual app Makefiles use Arduino for it's preprocessing of .INO files)

### On Linux

- A working compiler toolchain, make, etc.
- libSDL2 and headers
- Arduino IDE
- You will need to tweak the Makefiles.

If someone wants to expand on this, please do.

### On Windows

Currently the low-level file APIs used may not work at all for Windows.  If someone is familiar with Windows filesystem APIs and wants to flesh that out more fully I'm open to a PR.  Discuss it with me first though (I'm on Slack) since I should probably clean up the SD stuff a bit more before anyone starts to contribute.
