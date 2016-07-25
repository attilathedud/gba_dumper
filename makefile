CC = gcc
CFLAGS = -Wall

DEPS = include/core.h include/searching.h include/output.h include/translate.h include/common.h include/utils/uthash.h include/utils/utils.h include/hashing.h include/input.h
SOURCES = src/rom_dumper.c src/core.c src/searching.c src/output.c src/translate.c src/hashing.c src/input.c src/utils/utils.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(SOURCES)
	$(CC) -o rom_dumper $(SOURCES) $(CFLAGS)

debug: $(SOURCES)
	$(CC) -o rom_dumper_debug $(SOURCES) -g $(CFLAGS)

.PHONY: clean

clean:
	rm -f src/*.o rom_dumper
	