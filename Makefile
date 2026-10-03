CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.0)

bmp-ascii: $(OBJ)
	$(CC) $(OBJ) -o bmp-ascii

build/%.0: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm bmp-ascii $(OBJ)
