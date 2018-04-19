all: prefixsum-omp.bin

prefixsum-omp.bin: prefixsum-omp.c
	gcc -std=c99 -O2 -fopenmp prefixsum-omp.c secret.c -o prefixsum-omp.bin

clean:
	rm -f prefixsum-omp.bin
