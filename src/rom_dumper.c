#ifndef COMMON_H
#define COMMON_H
	#include "../include/common.h"
#endif

#include "../include/rom_core.h"
#include "../include/dump_core.h"

#ifndef SEARCHING_H
#define SEARCHING_H
	#include "../include/searching.h"
#endif

#include "../include/input.h"
#include "../include/output.h"
#include "../include/translate.h"

void cleanup( rom_file *rom, dump_file *dump )
{
	if( rom != NULL && rom->rom_buffer != NULL )
	{
		free( rom->rom_buffer );
	}

	if( dump != NULL )
	{
		if( dump->rom_buffer != NULL )
		{
			free( dump->rom_buffer );
		}

		if( dump->translated_buffer != NULL )
		{
			free( dump->translated_buffer );
		}
	}
}

int main( int argc, char** argv ) 
{
	rom_file rom 								= { 0 };
	passed_options options 						= { 0 };

	dump_file dump 								= { 0 };

	if( 1 == handle_input( &rom, &dump, &options, argc, argv ) )
		return 1;

	if( rom.rom_path != NULL )
	{
		if( get_rom_length( &rom ) == -1 )
		{
			//no need to cleanup here as we haven't allocated anything yet
			fprintf( stderr, "Error getting rom length.\n" );
			return -1;
		}

		rom.rom_buffer = (unsigned char*) malloc( rom.rom_length + 1 );

		dump_rom_into_buffer( &rom );

		if( options.dump_rom_flag ) 
		{
			print_buffer_contents_f( &rom, 0 );

			cleanup( &rom, &dump );
			return 0;
		}

		if( options.relative_search_text != NULL )
		{
			match_info matches = { 0 };

			if( -1 == relative_search( &rom, &matches, options.relative_search_text, options.fuzz_value) )
			{
				fprintf( stderr, "Error occured while searching.\n" );

				cleanup( &rom, &dump );
				return -1;
			}

			print_match_list( &rom, &matches, strlen( options.relative_search_text ) );

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
						options.relative_search_text )) < 0 )
					{
						if( translation_return_info == -1 )
						{
							fprintf( stderr, "Error while generating the translation file.\n" );

							cleanup( &rom, &dump );
							return -1;
						}
						else if( translation_return_info == -2 )
						{
							fprintf( stderr, "Ambigious data set provided. Make sure character set provided are consistent.\n" );

							cleanup( &rom, &dump );
							return -1;
						}
					}
				}

				free( translate_file_path );
			}

			cleanup( &rom, &dump );
			return 0;
		}

		if( options.translation_file_arg != NULL )
		{
			if( -1 == read_translation_file( options.translation_file_arg ) )
			{
				fprintf( stderr, "Error while reading the translation file.\n" );

				cleanup( &rom, &dump );
				return -1;
			}

			if( -1 == create_translated_rom( &rom ) )
			{
				fprintf( stderr, "Error while creating the translated rom.\n" );

				cleanup( &rom, &dump );
				return -1;
			}

			cleanup( &rom, &dump );
			return 0;
		}		
	}
	else if( dump.dump_path != NULL && options.write_file_path != NULL && options.translation_file_arg != NULL )
	{
		if( -1 == read_translation_file( options.translation_file_arg ) )
		{
			fprintf( stderr, "Error while reading the translation file.\n" );

			cleanup( &rom, &dump );
			return -1;
		}

		if( -1 == get_dump_amount_of_lines( &dump ) )
		{
			fprintf( stderr, "Error while parsing the dump file.\n" );

			cleanup( &rom, &dump );
			return -1;
		}

		dump.rom_buffer 				= (unsigned char*) malloc( dump.rom_length );
		dump.translated_buffer 			= (unsigned char*) malloc( dump.translated_length );

		if( -1 == read_dump_file( &dump ) )
		{
			fprintf( stderr, "Error while reading the dump file.\n" );

			cleanup( &rom, &dump );
			return -1;
		}

		if( -1 == write_translated_dump( &dump, options.write_file_path ) )
		{
			fprintf( stderr, "Error while writing the new rom.\n" );

			cleanup( &rom, &dump );
			return -1;
		}

		cleanup( &rom, &dump );
		return 0;
	}

	cleanup( &rom, &dump );
	return 0;
}
