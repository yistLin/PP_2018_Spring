CC = gcc
CFLAGS = -std=c99 -O2
OBJS = getDeviceInfo-cl

all: $(OBJS)

%-cl: %.c
	$(CC) $(CFLAGS) $< -lOpenCL -o $@

clean:
	rm -f $(OBJS)
