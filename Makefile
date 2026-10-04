CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)

bmp-ascii: $(OBJ)
	$(CC) $(OBJ) -o bmp-ascii

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f bmp-ascii $(OBJ)
