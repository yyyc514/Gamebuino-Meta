CC=gcc
CPP=g++
LIBS=
INCLUDES=-I. -Istub `sdl2-config --cflags --libs`
SRC_ALL=$(wildcard utility/*.cpp) \
	$(wildcard utility/*/*.cpp) \
	$(wildcard utility/*/*/*.cpp) \
	$(wildcard utility/*/*/*/*.cpp) \
	stub/Print.cpp \
	stub/timekeeping.cpp \
	stub/WMath.cpp \
	Gamebuino-Meta.cpp

SRC=$(filter-out utility/Adafruit_ZeroDMA/% utility/SdFat/%,$(SRC_ALL))

# EXTRA_INCLUDES is passed from the sketch to allow inclusion of the
# local config-gamebuino.h if there is one
CFLAGS=-std=c++11  $(INCLUDES) $(EXTRA_INCLUDES) -w -g
SKETCH_NAME?='"""library.ino"""'
DEFINES=-DARDUINO=185 -D__arm__ -DSPI_HAS_TRANSACTION -D__SKETCH_NAME__=$(SKETCH_NAME)
OBJDIR=obj
BUILD_DIR=build
OFILES=$(foreach bname, $(notdir $(basename $(SRC))), $(BUILD_DIR)/$(bname).o) \
	build/font3x5.o
# OFILES=$(foreach bname, $(basename $(SRC)), $(BUILD_DIR)/$(bname).o)


# /Applications/Arduino.app/Contents/Java/arduino-builder -hardware /Applications/Arduino.app/Contents/Java/hardware -hardware /Users/jgoebel/Documents/Arduino/hardware -tools /tmp -fqbn=gamebuino_meta:samd:gamebuino_meta_native -ide-version=10805 -build-path build /Users/jgoebel/git/arduino/meta-solitaire/meta-solitaire/meta-solitaire.ino

# vpath %.cpp utility/**/**
# vpath %.cpp utility/**

# $(warning $(OFILES))

all: $(BUILD_DIR) $(BUILD_DIR)/meta.o

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

build/font3x5.o: utility/Graphics/font3x5.c
	gcc -c $< $(INCLUDES) -o $@

# obj/%.o : %.cpp
	# g++ $< $(CFLAGS) $(LIBS) $(DEFINES) -o $@

# $(warning $(foreach dname, $(dir $(SRC)), $(dname)))

# $(BUILD_DIR)/%.o : %.cpp
# $(OFILES) : %.cpp
# $(OFILES)/%.o: $(SRC)/%.cpp build/font3x5.o
$(BUILD_DIR)/%.o : stub/%.cpp
	g++ -c $^ $(CFLAGS) $(LIBS) $(DEFINES) -o $@

$(BUILD_DIR)/%.o : utility/%.cpp
	g++ -c $^ $(CFLAGS) $(LIBS) $(DEFINES) -o $@

$(BUILD_DIR)/FakeSD.o: utility/FakeSD/FakeSD.cpp utility/FakeSD/*.h
	g++ -c $< $(CFLAGS) $(LIBS) $(DEFINES) -o $@

$(BUILD_DIR)/File.o: utility/FakeSD/File.cpp  utility/FakeSD/*.h
	g++ -c $< $(CFLAGS) $(LIBS) $(DEFINES) -o $@

$(BUILD_DIR)/%.o : utility/**/%.cpp
	g++ -c $^ $(CFLAGS) $(LIBS) $(DEFINES) -o $@

$(BUILD_DIR)/%.o : utility/**/**/%.cpp
	g++ -c $^ $(CFLAGS) $(LIBS) $(DEFINES) -o $@

$(BUILD_DIR)/%.o : utility/**/**/**/%.cpp
	g++ -c $^ $(CFLAGS) $(LIBS) $(DEFINES) -o $@

$(BUILD_DIR)/%.o : Gamebuino-Meta.cpp Gamebuino-Meta.h
	g++ -c -o $@ $(CFLAGS) $(LIBS) $(DEFINES) $(filter %.cpp %.o, $^)

$(BUILD_DIR)/meta.o: $(OFILES)
# $(OBJDIR) meta.so: $(SRC) Gamebuino-Meta.h
	# g++ -shared -c $(SRC) $(CFLAGS) $(LIBS) $(DEFINES)
	# g++ -c -o $@ $(CFLAGS) $(LIBS) $(DEFINES) $(filter %.cpp %.o, $^)
	ar ru $@ $^
	ranlib $@

clean:
	rm -rf build/*.o
	rm -rf build/*.so
	rmdir build
