all: redblue.bin

redblue.bin: main.c
	gcc -std=c99 -O2 -Wall -fopenmp main.c -o redblue.bin

clean:
	rm -f redblue.bin
