#include <stdio.h>

#ifndef COMMON_H
#define COMMON_H
	#include "common.h"
#endif

#ifndef SEARCHING_H
#define SEARCHING_H
	#include "searching.h"
#endif

#ifndef HASHING_H
#define HASHING_H
	#include "hashing.h"
#endif

void print_buffer_contents_f( rom_file*, unsigned long );
void reprint_dump_contents( dump_file *, unsigned long );
void print_match_list( rom_file*, match_info*, int );
void print_buffer_as_bytes( FILE*, unsigned char*, long );
void print_character_translation( FILE *, char );
void print_newline_or_character( int, char );
