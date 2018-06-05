CC = gcc
CFLAGS = -std=c99 -O2 -fopenmp
OBJ = main-cl

all: $(OBJ)

$(OBJ): main.c
	$(CC) $(CFLAGS) $< -lOpenCL -o $@

clean:
	rm -f $(OBJ)
