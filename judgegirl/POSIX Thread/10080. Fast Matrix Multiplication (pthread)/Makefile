all: matrix.bin

matrix.bin: matrix.h matrix.c main.c
	gcc -std=c99 -O2 -lpthread -lm matrix.c main.c -o matrix.bin

clean:
	rm -f matrix.bin
