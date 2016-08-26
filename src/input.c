#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#include "../include/input.h"

char* help_text 				=  "gba_dumper v1.0 by Nathan Tucker.\n\n" \
								   "gba_dumper is a set of utilites designed to make text modifications for Game Boy Advanced (gba) roms easier. " \
								   "Since gba games don't use ascii and instead each use their own homebrew-encoding, gba_dumper attempts to streamline "\
								   "the approach of translating rom byte-codes into human-readable characters.\n\n" \
								   "The intended workflow is:\n" \
								   "1. Search for a string of text\n2. Using the matches, let gba_dumper create a translation file." \
								   "If there are multiple conflicting matches, gba_dumper will prompt you to choose the matching set.\n" \
								   "3. Using that translation file, dump a \"translated\" version of the rom.\n" \
								   "4. Using that dump, add special characters (:!@#, spaces, etc.) to the translation file and retranslate.\n" \
								   "5. Using the dump, note the areas of memory that hold strings you want to edit, and note the character used as a newline character in the game.\n" \
								   "6. Dump the memory noted in 5, edit all the strings you want, and reupload them to a new dump.\n" \
								   "7. Using the modified dump, create a new rom.\n\n" \
								   "Options:\n" \
								   "-f\tPath to the rom to perform operations on. This must be the uncompressed .gba file.\n" \
								   "-r\tText to search for within the rom.\n"\
								   "-z\tWhen searching for text, the amount of fuzz (or allowed error) to apply.\n"\
								   "-d\tDump the formatted bytes of the rom.\n"\
								   "-t\tPath to a valid translation file, which is newline separated list of byte_value : character values (e.g., 00FF : b).\n"\
								   "-m\tPath to a valid dump file, which is automatically generated after translating the rom.\n"\
								   "-b\tThe two-byte value that the game uses for segment breaks within contiguous memory spaces\n"\
								   "-a\tWhen dumping strings, the start address of memory to start dumping at.\n"\
								   "-e\tWhen dumping strings, the end address of memory to end dumping at.\n"\
								   "-s\tPath to a valid strings file to write back to the dump, which is a newline separated list of strings.\n\n"\
								   "Example Usage:\n"\
								   "Step 1:\t$ gba_dumper -f roms/rom.gba -r \"Example\"\n" \
								   "\t1: 0x00721940	C100D300D300C900C700CE00C500C400\n"\
								   "\tAttempt to generate a translation file? (if yes, type file name. For no, type nothing):\n"\
								   "\trom.tra\n"\
								   "Step 3:\t $ gba_dumper -f roms/rom.gba -t rom.tra > rom.dmp\n"\
								   "Step 5:\t $ gba_dumper -m rom.dmp -b 00FF -a 0x00721920 -e 0x007226A0 > rom.str\n"\
								   "Step 6:\t $ gba_dumper -m rom.dmp -s rom.str -b 00FF -a 0x00721920 -e 0x007226A0 > rom2.dmp\n"\
								   "Step 7:\t $ gba_dumper -m rom2.dmp -t rom.tra > rom2.gba\n";

/*! 
*	Helper function to safety extract a numerical value from a passed character array.
*
*	Sets errno with the result of strtol.
*
*	Returns the value on success, -1 on failure.
*/
unsigned long get_long_value_from_optarg( char* optarg, int base )
{
	char *temp_end_for_conversion;

	errno = 0;

	unsigned long temp_value = strtol( optarg, &temp_end_for_conversion, base);
	if( temp_end_for_conversion != optarg && errno != ERANGE && (temp_value >= INT_MIN || temp_value <= INT_MAX))
	{
		return temp_value;
	}

	return -1;
}

/*!
*	Given pointers to the rom/dump files, along with the argument vector, populate the options structure.
*
*	rom:		Pointer to an allocated rom_file.
*	dump:		Pointer to an allocated dump_file.
*	options:	Pointer to an allocation options structure that will be filled with relevant data.
*	argc, argv:	The argument count and vector retrieved from main.
*
*	Returns 0 if success, 1 if invalid arguments or error parsing the arguments.
*/
int handle_input( rom_file *rom, dump_file *dump, passed_options* options, int argc, char** argv )
{
	int cur_arg 								= 0;

	if( rom == NULL || options == NULL || dump == NULL )
		return 1;

	while( (cur_arg = getopt( argc, argv, "hf:r:dz:t:m:s:b:a:e:" ) ) != -1 )
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
			case 'm':
				dump->dump_path = optarg;
				break;
			case 'b':
				options->rom_string_break = optarg;
				break;
			case 's':
				options->strings_file_arg = optarg;
				break;
			case 'a':
				options->start_address = get_long_value_from_optarg( optarg, 16 );
				break;
			case 'e':
				options->end_address = get_long_value_from_optarg( optarg, 16 );
				break;
			case 'z':
				options->fuzz_value = (int)get_long_value_from_optarg( optarg, 10 );
				break;
			case 'h':
				printf( "%s\n", help_text );
				break;
			case '?':
				switch( optopt )
				{
					case 'f':
					case 'm':
					case 't':
					case 's':
						fprintf( stderr, "Option %c requires a valid path to a file.\n", optopt );
						break;
					case 'r':
						fprintf(stderr, "Option %c requires a valid string to search.\n", optopt);
						break;
					case 'z':
						fprintf(stderr, "Option %c requires a valid integer to fuzz with.\n", optopt);
						break;
					case 'b':
						fprintf(stderr, "Option %c requires a valid break character (2 byte hex value).\n", optopt);
						break;
					case 'a':
					case 'e':
						fprintf(stderr, "Option %c requires a valid hex address.\n", optopt);
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
