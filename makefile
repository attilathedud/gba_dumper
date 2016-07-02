CC = gcc
CFLAGS = -Wall

DEPS = include/core.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: src/rom_dumper.c src/core.c
	$(CC) -o rom_dumper src/rom_dumper.c src/core.c $(CFLAGS)

.PHONY: clean

clean:
	rm -f src/*.o rom_dumper
	