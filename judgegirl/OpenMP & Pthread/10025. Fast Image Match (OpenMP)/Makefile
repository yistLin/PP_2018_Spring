all: match.bin

match.bin: main.c
	gcc -std=c99 -O2 -fopenmp main.c -o match.bin

clean:
	rm -f match.bin
