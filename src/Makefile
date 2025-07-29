HEXMAP=HexMap.cpp
HEXMAP_OUTPUT=HexMap.exe
DEBUG=-g -O0
LIBS=-O3 -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDES=-I/usr/include/SDL2
UNAME_S=$(shell uname -s)
UNAME_M=$(shell uname -m)
DEBIAN_ARCH=amd64
DEBIAN=blood-sword.$(DEBIAN_ARCH)

ifeq ($(UNAME_S),Darwin)
	DEBIAN_ARCH=
	DEBIAN=
	LLVM=$(shell brew --prefix llvm)
	CC=$(LLVM)/bin/clang++
	LIBS += -stdlib=libc++ -Wl,-dead_strip -fdata-sections -ffunction-sections -L$(LLVM)/lib/c++ -L/usr/local/lib
	INCLUDES += -I/usr/local/include/SDL2
else ifeq ($(UNAME_M),aarch64)
	DEBIAN_ARCH=arm64
	DEBIAN=blood-sword.$(DEBIAN_ARCH)
	CC=g++
	LIBS += -lstdc++fs
else ifeq ($(UNAME_M),aarch32)
	DEBIAN_ARCH=
	DEBIAN=
	LIBS += -lstdc++fs
	CC=g++
else
	CC=clang++
	LIBS += -fdata-sections -ffunction-sections -lstdc++fs -Wl,--gc-sections
endif

.PHONY: all clean

all: clean hex-map

hex-map:
	$(CC) -Wall $(HEXMAP) $(LIBS) $(INCLUDES) -o $(HEXMAP_OUTPUT)

clean-hex-map:
	rm -f $(HEXMAP_OUTPUT) *.o

clean: clean-hex-map
