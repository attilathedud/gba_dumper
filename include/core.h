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

unsigned long get_rom_length( rom_file* );
int dump_rom_into_buffer( rom_file* );

int create_translation_file( char*, int );
int read_translation_file( char*, int );
int create_translated_rom( rom_file*, int );

int get_dump_amount_of_lines( dump_file* );
int read_dump_file( dump_file* );
