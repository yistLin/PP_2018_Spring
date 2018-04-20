all: lcs.bin

lcs.bin: main.c
	gcc -std=c99 -O2 -fopenmp main.c -o $@

clean:
	rm -f lcs.bin
