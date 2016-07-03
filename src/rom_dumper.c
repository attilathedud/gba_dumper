#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "../include/core.h"
#include "../include/searching.h"

#define MAX_LOCATION_MATCHES 25

int main( int argc, char** argv ) 
{
	int cur_arg 					= 0;
	int dump_rom_flag				= 0;
	int unicode_flag				= 0;
	int fuzz_value					= 0;

	char *rom_path					= NULL;
	char *relative_search_text		= NULL;
	char *temp_end_for_conversion 	= NULL;

	unsigned char *rom_buffer		= NULL;

	unsigned long rom_length		= 0;
	
	while( (cur_arg = getopt( argc, argv, "f:r:duz:" ) ) != -1 )
	{
		switch( cur_arg )
		{
			case 'f':
				rom_path = optarg;
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

	if( rom_path != NULL )
	{
		rom_length = get_rom_length( rom_path );

		rom_buffer = (unsigned char*) malloc( rom_length + 1 );

		dump_rom_into_buffer( rom_path, rom_buffer, rom_length );

		if( dump_rom_flag ) 
		{
			print_buffer_contents_f( rom_buffer, rom_length );
		}

		if( relative_search_text != NULL )
		{
			unsigned long location_matches[ MAX_LOCATION_MATCHES ] = { 0 };

			int amount_of_matches = relative_search( rom_buffer, rom_length, location_matches, MAX_LOCATION_MATCHES, 
				relative_search_text, unicode_flag, fuzz_value);

			for( int i = 0; i < amount_of_matches; i++ )
			{
				printf("0x%.8lX\t", location_matches[ i ]);
				for( int c = 0; c < strlen( relative_search_text ) * (unicode_flag ? 2 : 1); c++ )
				{
					printf("%.2X", rom_buffer[ location_matches[ i ] + c ]);
				}

				printf("\n");
			}
		}

		free( rom_buffer );
	}

	return 0;
}
