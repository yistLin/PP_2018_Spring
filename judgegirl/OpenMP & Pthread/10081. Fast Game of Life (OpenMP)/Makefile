all: game-of-life.bin

game-of-life.bin: main.c
	gcc -std=c99 -O2 -fopenmp main.c -o game-of-life.bin

clean:
	rm -f game-of-life.bin
