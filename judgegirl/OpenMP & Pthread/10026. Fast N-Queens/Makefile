all: n-queens.bin

n-queens.bin: main.c
	gcc -std=c99 -O2 -fopenmp main.c -o n-queens.bin

clean:
	rm -f n-queens.bin
