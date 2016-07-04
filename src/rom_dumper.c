#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#ifndef COMMON_H
#define COMMON_H
	#include "../include/common.h"
#endif

#include "../include/core.h"

#ifndef SEARCHING_H
#define SEARCHING_H
	#include "../include/searching.h"
#endif

#include "../include/output.h"
#include "../include/translate.h"

int main( int argc, char** argv ) 
{
	int cur_arg 					= 0;
	int dump_rom_flag				= 0;
	int unicode_flag				= 0;
	int fuzz_value					= 0;

	char *relative_search_text		= NULL;
	char *temp_end_for_conversion 	= NULL;

	char translate_file_path[ MAX_PATH_LENGTH ] = { 0 };

	rom_file rom = { 0 };
	
	while( (cur_arg = getopt( argc, argv, "f:r:duz:" ) ) != -1 )
	{
		switch( cur_arg )
		{
			case 'f':
				rom.rom_path = optarg;
				break;
			case 'r':
				relative_search_text = optarg;
				break;
			case 'd':
				dump_rom_flag = 1;
				break;
			case 'u':
				unicode_flag = 1;
				break;
			case 'z':
				errno = 0;

				long temp_value = strtol( optarg, &temp_end_for_conversion, 10);
				if( temp_end_for_conversion != optarg && errno != ERANGE && (temp_value >= INT_MIN || temp_value <= INT_MAX))
				{
					fuzz_value = (int)temp_value;
				}
				break;
			case '?':
				if( optopt == 'f' )
				{
					fprintf( stderr, "Option %c requires a valid path to a rom.\n", optopt );
				}
				else if( optopt == 'r' )
				{
					fprintf(stderr, "Option %c requires a valid string to search.\n", optopt);
				}
				else if( optopt == 'z' )
				{
					fprintf(stderr, "Option %c requires a valid integer to fuzz with.\n", optopt);
				}
				else if( isprint( optopt ) )
				{
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				}
				else
				{
					fprintf(stderr, "Unknown option character '%x'.\n", optopt);
				}

				return 1;
			default:
				abort();
		}
	}

	if( rom.rom_path != NULL )
	{
		if( get_rom_length( &rom ) == -1 )
		{
			printf("Error getting rom length. Check your path.\n");
			return -1;
		}

		rom.rom_buffer = (unsigned char*) malloc( rom.rom_length + 1 );

		dump_rom_into_buffer( &rom );

		if( dump_rom_flag ) 
		{
			print_buffer_contents_f( &rom, 0 );
		}

		if( relative_search_text != NULL )
		{
			match_info matches = { 0 };

			if( relative_search( &rom, &matches, relative_search_text, unicode_flag, fuzz_value) == -1 )
			{
				printf("Error occured while searching.\n");
				return -1;
			}

			print_match_list( &rom, &matches, strlen( relative_search_text ), unicode_flag );

			if( matches.amount_of_matches > 0 )
			{
				printf("Attempt to generate a translation file? (if yes, type file name. For no, type nothing):\n");
				fgets( translate_file_path, MAX_PATH_LENGTH, stdin );

				if( strlen( translate_file_path ) > 0 )
				{
					
				}
			}
		}

		free( rom.rom_buffer );
	}

	return 0;
}
