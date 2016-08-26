#ifndef COMMON_H
#define COMMON_H
	#include "common.h"
#endif

/*!
*	To separate out parsing the arguments from the main application flow, all code related to handling arguments is placed with input.h/.c.
*/

/*!
*	This structure will be populated in handle_input and returned to the main caller. It is then used to control program flow via set flags.
*/
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

/*!
*	Given pointers to the rom/dump files, along with the argument vector, populate the options structure.
*/
int handle_input( rom_file*, dump_file*, passed_options*, int, char** );
