all: prefixsum-seq prefixsum-pthread

prefixsum-pthread: prefixsum-pthread.c
	gcc -std=c99 -O2 -pthread prefixsum-pthread.c secret.c -o prefixsum-pthread

prefixsum-seq: prefixsum-seq.c
	gcc -std=c99 -O2 -pthread prefixsum-seq.c secret.c -o prefixsum-seq

test1:
	time ./prefixsum-seq < 0.in

test2:
	time ./prefixsum-pthread < 0.in

clean:
	rm -f prefixsum-seq prefixsum-pthread
