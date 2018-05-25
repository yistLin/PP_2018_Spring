CC = gcc
CFLAGS = -std=c99 -O2 -fopenmp
OBJS = main-cl

all: $(OBJS)

%-cl: %.c
	$(CC) $(CFLAGS) $< -lOpenCL -o $@

clean:
	rm -f $(OBJS)
