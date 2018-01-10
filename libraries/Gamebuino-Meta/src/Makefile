CC=gcc
CPP=g++
LIBS=
INCLUDES=-I. -Istub `sdl2-config --cflags --libs`
SRC=$(wildcard utility/*.cpp) \
	$(wildcard utility/*/*.cpp) \
	$(wildcard utility/*/*/*.cpp) \
	$(wildcard utility/*/*/*/*.cpp) \
	stub/Print.cpp \
	stub/time.cpp \
	stub/WMath.cpp


CFLAGS=-std=c++11  $(INCLUDES) -w -g
DEFINES=-DARDUINO=185 -D__arm__
OBJDIR=obj
BUILD_DIR=build
OFILES=$(foreach bname, $(notdir $(basename $(SRC))), $(BUILD_DIR)/$(bname).o) \
	build/font3x5.o
# OFILES=$(foreach bname, $(basename $(SRC)), $(BUILD_DIR)/$(bname).o)

# vpath %.cpp utility/**/**
# vpath %.cpp utility/**

# $(warning $(OFILES))

all: solitaire

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

$(BUILD_DIR)/%.o : utility/**/%.cpp
	g++ -c $^ $(CFLAGS) $(LIBS) $(DEFINES) -o $@

$(BUILD_DIR)/%.o : utility/**/**/%.cpp
	g++ -c $^ $(CFLAGS) $(LIBS) $(DEFINES) -o $@

$(BUILD_DIR)/%.o : utility/**/**/**/%.cpp
	g++ -c $^ $(CFLAGS) $(LIBS) $(DEFINES) -o $@


meta.so: $(OFILES) Gamebuino-Meta.cpp Gamebuino-Meta.h
# $(OBJDIR) meta.so: $(SRC) Gamebuino-Meta.h
	# g++ -shared -c $(SRC) $(CFLAGS) $(LIBS) $(DEFINES)
	g++ -shared -o $@ $(CFLAGS) $(LIBS) $(DEFINES) $(filter %.cpp %.o, $^)

SOLITAIRE_SRC=$(wildcard meta-solitaire/meta-solitaire/*.cpp)

solitaire: $(SOLITAIRE_SRC) meta.so stub/core.cpp
	g++ -o $@ $^ $(CFLAGS) $(LIBS) $(DEFINES)

clean:
	rm -rf build/*.o
	rm -rf build/*.so
	rm solitaire
