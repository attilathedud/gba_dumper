CC = gcc
CFLAGS = -Wall

DEPS = include/core.h include/searching.h include/output.h include/common.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: src/rom_dumper.c src/core.c src/searching.c src/output.c 
	$(CC) -o rom_dumper src/rom_dumper.c src/core.c src/searching.c src/output.c  $(CFLAGS)

.PHONY: clean

clean:
	rm -f src/*.o rom_dumper
	