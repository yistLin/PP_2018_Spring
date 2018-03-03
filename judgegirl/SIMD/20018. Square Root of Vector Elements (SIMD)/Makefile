all: main

main: main.c VSQRT.o
	gcc -Wall -std=c99 -O3 main.c VSQRT.o -o main -lm

VSQRT.o: VSQRT.c
	gcc -Wall -std=c99 -O3 -msse -mavx VSQRT.c -c -o VSQRT.o -lm

test:
	time -p ./main

