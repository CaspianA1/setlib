CC = clang
OUT = example
CFLAGS = -O3 -o $(OUT) $(OUT).c

all:
	$(CC) $(CFLAGS)