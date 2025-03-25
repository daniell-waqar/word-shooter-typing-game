CC = g++
CFLAGS = -Iinclude -std=c++17 -Wall $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image

all: WordShooter

WordShooter: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o WordShooter $(LDFLAGS)

clean:
	rm -f WordShooter
