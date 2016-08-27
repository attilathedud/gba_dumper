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
*	Given a path to a rom, determine its size.
*/
unsigned long get_rom_length( rom_file* );

/*!
*	Given a path to a rom, read the rom into an allocated buffer.
*/
int dump_rom_into_buffer( rom_file* );

/*!
*	Given a path, create a translation file from the current byte->readable hash.
*/
int create_translation_file( char* );

/*!
*	Given a path, fill in the byte->readable and readable->byte hashes with information from the translation file.
*/
int read_translation_file( char* );

/*!
*	Given a pointer to a rom file, create a translated dump of the rom.
*/
int create_translated_rom( rom_file* );


