all: main

main: main.c DRS.c
	gcc -std=c99 -Ofast -march=native DRS.c -c -o DRS.o
	gcc -std=c99 -Ofast -march=native main.c DRS.o -o main
