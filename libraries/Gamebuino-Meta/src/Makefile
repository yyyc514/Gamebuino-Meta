LIBS=
INCLUDES=-I. -Istub `sdl2-config --cflags --libs`
SRC=Gamebuino-Meta.cpp \
	$(wildcard utility/*.cpp) \
	$(wildcard utility/*/*.cpp) \
	$(wildcard utility/*/*/*.cpp) \
	$(wildcard utility/*/*/*/*.cpp) \
	stub/Print.cpp \
	stub/time.cpp \
	stub/WMath.cpp \
	font3x5.o

CFLAGS=-std=c++11  $(INCLUDES) -w -g
DEFINES=-DARDUINO=185 -D__arm__

all: solitaire

font3x5.o: utility/Graphics/font3x5.c
	gcc -c $< $(INCLUDES)

meta.so: $(SRC) Gamebuino-Meta.h
	g++ -shared -o $@ $(SRC) $(CFLAGS) $(LIBS) $(DEFINES)

SOLITAIRE_SRC=$(wildcard meta-solitaire/meta-solitaire/*.cpp)

solitaire: $(SOLITAIRE_SRC) meta.so stub/core.cpp
	g++ -o $@ $^ $(CFLAGS) $(LIBS) $(DEFINES)

clean:
	rm solitaire
	rm meta.so
	rm font3x5.o
