CC = gcc
CFLAGS = -Wall -Wextra
SRC = main.c canvas.c
OUT = build/bin/o.out

all:
	mkdir -p build/bin
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf build

run:
	$(OUT)
