#ifndef COMMON_H
#define COMMON_H
	#include "common.h"
#endif

#ifndef OUTPUT_H
#define OUTPUT_H
	#include "output.h"
#endif

#ifndef HASHING_H
#define HASHING_H
	#include "hashing.h"
#endif

/*!
*	Given a dump file, calculate the amount of lines in the file so buffers for the rom and text sections can be alloc'd.
*/
int get_dump_amount_of_lines( dump_file* );

/*!
*	Given a dump file, parse the file and fill the dump_file struct with data.
*/
int read_dump_file( dump_file* );

/*!
*	Give a dump file, write any changes between the rom and text sections to a new rom. Requires the byte_to_readable hash to be filled.
*/
int write_translated_dump( dump_file* );

/*!
*	Given a dump file, start and end addresses, dump all the text between start and end into a newline separated list.
*/
int write_dump_strings( dump_file *, unsigned long, unsigned long, char* );

/*!
*	Given a dump file, strings file, start and end addresses, reupload all the strings to the dump's text section.
*/
int read_and_translate_dump_strings( dump_file *, char*, unsigned long, unsigned long, char* );
