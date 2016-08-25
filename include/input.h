#ifndef COMMON_H
#define COMMON_H
	#include "common.h"
#endif

typedef struct {
	int dump_rom_flag;
	int fuzz_value;

	char *relative_search_text;
	char *translation_file_arg;
	char *strings_file_arg;
	char *rom_string_break;

	unsigned long start_address;
	unsigned long end_address;
} passed_options;

int handle_input( rom_file*, dump_file*, passed_options*, int, char** );
