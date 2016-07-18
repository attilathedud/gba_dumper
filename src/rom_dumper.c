#ifndef COMMON_H
#define COMMON_H
	#include "../include/common.h"
#endif

#include "../include/core.h"

#ifndef SEARCHING_H
#define SEARCHING_H
	#include "../include/searching.h"
#endif

#include "../include/input.h"
#include "../include/output.h"
#include "../include/translate.h"

int main( int argc, char** argv ) 
{
	rom_file rom 								= { 0 };
	passed_options options 						= { 0 };

	if( 1 == handle_input( &rom, &options, argc, argv ) )
		return 1;

	if( rom.rom_path != NULL )
	{
		if( get_rom_length( &rom ) == -1 )
		{
			printf("Error getting rom length. Check your path.\n");
			return -1;
		}

		rom.rom_buffer = (unsigned char*) malloc( rom.rom_length + 1 );

		dump_rom_into_buffer( &rom );

		if( options.dump_rom_flag ) 
		{
			print_buffer_contents_f( &rom, 0, options.unicode_flag );

			free( rom.rom_buffer );
			return 0;
		}

		if( options.relative_search_text != NULL )
		{
			match_info matches = { 0 };

			if( relative_search( &rom, &matches, options.relative_search_text, options.unicode_flag, options.fuzz_value) == -1 )
			{
				printf("Error occured while searching.\n");
				return -1;
			}

			print_match_list( &rom, &matches, strlen( options.relative_search_text ), options.unicode_flag );

			if( matches.amount_of_matches > 0 )
			{
				size_t file_path_len =						 0;

				char *translate_file_path;

				printf("Attempt to generate a translation file? (if yes, type file name. For no, type nothing):\n");
				getline( &translate_file_path, &file_path_len, stdin );

				//strip newline characters from input name
				//see https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input/28462221#28462221
				translate_file_path[ strcspn( translate_file_path, "\r\n" ) ] = 0;

				if( strlen( translate_file_path ) > 0 )
				{
					int translation_return_info = 0;

					if( (translation_return_info = generate_translation_set_from_matches( &rom, translate_file_path, &matches, 
						options.relative_search_text, options.unicode_flag )) < 0 )
					{
						if( translation_return_info == -1 )
						{
							printf( "Error while generating the translation file.\n");
							return -1;
						}
						else if( translation_return_info == -2 )
						{
							printf( "Ambigious data set provided. Make sure character set provided are consistent.\n");
							return 0;
						}
					}
				}

				free( translate_file_path );
			}

			free( rom.rom_buffer );
			return 0;
		}

		if( options.translation_file_arg != NULL )
		{
			if( -1 == read_translation_file( options.translation_file_arg, options.unicode_flag ) )
			{
				printf( "Error while reading the translation file.\n" );
			}

			create_translated_rom( &rom, options.unicode_flag );

			free( rom.rom_buffer );
			return 0;
		}		
	}

	return 0;
}
