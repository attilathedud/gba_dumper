CC = gcc
CFLAGS = -Wall

DEPS = include/core.h include/searching.h include/output.h include/translate.h include/common.h include/utils/uthash.h include/hashing.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: src/rom_dumper.c src/core.c src/searching.c src/output.c src/translate.c src/hashing.c
	$(CC) -o rom_dumper src/rom_dumper.c src/core.c src/searching.c src/output.c src/translate.c src/hashing.c $(CFLAGS)

debug: src/rom_dumper.c src/core.c src/searching.c src/output.c src/translate.c src/hashing.c
	$(CC) -o rom_dumper_debug src/rom_dumper.c src/core.c src/searching.c src/output.c src/translate.c src/hashing.c -g $(CFLAGS)

.PHONY: clean

clean:
	rm -f src/*.o rom_dumper
	