#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#include "../include/input.h"

int handle_input( rom_file *rom, dump_file *dump, passed_options* options, int argc, char** argv )
{
	int cur_arg 								= 0;

	char *temp_end_for_conversion;

	if( rom == NULL || options == NULL || dump == NULL )
		return 1;

	while( (cur_arg = getopt( argc, argv, "f:r:dz:t:m:o:" ) ) != -1 )
	{
		switch( cur_arg )
		{
			case 'f':
				rom->rom_path = optarg;
				break;
			case 'r':
				options->relative_search_text = optarg;
				break;
			case 'd':
				options->dump_rom_flag = 1;
				break;
			case 't':
				options->translation_file_arg = optarg;
				break;
			case 'o':
				options->write_file_path = optarg;
				break;
			case 'm':
				dump->dump_path = optarg;
				break;
			case 'z':
				errno = 0;

				long temp_value = strtol( optarg, &temp_end_for_conversion, 10);
				if( temp_end_for_conversion != optarg && errno != ERANGE && (temp_value >= INT_MIN || temp_value <= INT_MAX))
				{
					options->fuzz_value = ( int ) temp_value;
				}
				break;
			case '?':
				switch( optopt )
				{
					case 'f':
						fprintf( stderr, "Option %c requires a valid path to a rom.\n", optopt );
						break;
					case 'r':
						fprintf(stderr, "Option %c requires a valid string to search.\n", optopt);
						break;
					case 'z':
						fprintf(stderr, "Option %c requires a valid integer to fuzz with.\n", optopt);
						break;
					case 't':
						fprintf(stderr, "Option %c requires a valid path to a translation file.\n", optopt);
						break;
					case 'o':
						fprintf(stderr, "Option %c requires a valid path to an output file.\n", optopt);
						break;
					case 'm':
						fprintf(stderr, "Option %c requires a valid path to a dump file.\n", optopt);
						break;
					default:
						if ( isprint( optopt ) )
						{
							fprintf(stderr, "Unknown option '-%c'.\n", optopt);
						}
						else
						{
							fprintf(stderr, "Unknown option character '%x'.\n", optopt);
						}
						
						break;
				}

				return 1;
			default:
				abort();
		}
	}

	return 0;
}
