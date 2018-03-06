all: matrix.out

matrix.out: matrix.h matrix.c main.c
	gcc -std=c99 -O2 -lpthread -lm matrix.c main.c -o matrix.out

clean:
	rm -f matrix.out
