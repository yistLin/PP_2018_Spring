all: main.c
	gcc -std=c99 -O3 -march=native main.c -lm -o main

test:
	time ./main < 0.in

clean:
	rm -f ./main
