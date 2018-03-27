all: prefix_sum.bin

prefix_sum.bin: main.c
	gcc -std=c99 -O2 main.c -o prefix_sum.bin

run:
	./prefix_sum.bin < 0.in

clean:
	rm -f prefix_sum.bin
