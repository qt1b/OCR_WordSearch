CC = gcc

SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LIBS := $(shell sdl2-config --libs) -lSDL2_image -lm

SRC = image.c

OUT = image

$(OUT): $(SRC)
	$(CC) $(SRC) $(SDL2_CFLAGS) $(SDL2_LIBS) -o $(OUT)

.PHONY: clean
clean:
	rm -f $(OUT)
