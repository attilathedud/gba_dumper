#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/core.h"

int main( int argc, char** argv ) 
{
	int cur_arg 					= 0;
	int dump_rom_flag				= 0;

	char *rom_path					= NULL;
	char *relative_search_text		= NULL;

	unsigned char *rom_buffer		= NULL;

	unsigned long rom_length		= 0;
	
	while( (cur_arg = getopt( argc, argv, "f:r:d" ) ) != -1 )
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
			case '?':
				if( optopt == 'f' )
				{
					fprintf( stderr, "Option %c requires a valid path to a rom.\n", optopt );
				}
				else if( optopt == 'r' )
				{
					fprintf(stderr, "Option %c requires a valid string to search.\n", optopt);
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

		free( rom_buffer );
	}

	return 0;
}
