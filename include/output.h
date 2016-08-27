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

/*!
*	Contains all the output related functionality (anything involving writing to stdout).
*/

/*!
*	Given a pointer to a rom and its length, print out its contents in a formatted manner.
*/
void print_buffer_contents_f( rom_file*, unsigned long );

/*!
*	Given a pointer to a dump file and its length, print out its contents in a formatted manner.
*/
void reprint_dump_contents( dump_file *, unsigned long );

/*!
*	Given a pointer to a rom and a set of matches, print out the list of matches and their locations.
*/
void print_match_list( rom_file*, match_info*, int );

/*!
*	Given a buffer, print out its contents formatted as 2 character bytes.
*/
void print_buffer_as_bytes( FILE*, unsigned char*, long );

/*!
*	Given a character, print it in the translation file format.
*/
void print_character_translation( FILE *, char );

/*!
*	Given a conditional and character, print a newline if the conditional is true, the character otherwise.
*/
void print_newline_or_character( int, char );
