#include <stdio.h>
#include <stdlib.h>

#include "../include/rom_core.h"
#include "../include/utils/utils.h"

/*
* TODO: 
* 	Remove magic numbers
*	Fix crash while translating ascii rom
*/

unsigned long get_rom_length( rom_file *rom )
{
	FILE *rom_file						= NULL;

	if( rom == NULL || rom->rom_path == NULL )
		return -1;

	rom_file = fopen( rom->rom_path, "rb" );
	if( rom_file == NULL )
		return -1;

	fseek( rom_file, 0, SEEK_END );
	rom->rom_length = ftell( rom_file );
	fseek( rom_file, 0, SEEK_SET );

	fclose( rom_file );

	return rom->rom_length;
}

int dump_rom_into_buffer( rom_file *rom )
{
	FILE *rom_file						= NULL;

	if( rom == NULL || rom->rom_path == NULL || rom->rom_buffer == NULL )
		return -1;

	rom_file = fopen( rom->rom_path, "rb" );
	if( rom_file == NULL )
		return -1;

	fread( rom->rom_buffer, rom->rom_length, 1, rom_file );

	fclose( rom_file );

	return 0;
}

int create_translation_file( char *filename )
{
	FILE *translation_file 				= NULL;

	byte_to_readable_set *s 			= NULL;

	if( filename == NULL )
		return -1;

	translation_file = fopen( filename, "w" );
	if( translation_file == NULL )
		return -1;

	for( s = get_byte_to_readable_hash(); s != NULL; s = s->hh.next ) 
	{
        print_buffer_as_bytes( translation_file, s->byte_value, 2 );
        print_character_translation( translation_file, s->readable);
    }

    fclose( translation_file );

	return 0;
}

int read_translation_file( char *filename )
{
	FILE *translation_file 				= NULL;

	char *cur_line						= NULL;
	size_t cur_len 						= 0;

	unsigned char byte_hex[ 2 ]			= { 0 };
	unsigned char byte_literal[ 4 ]		= { 0 };
	char readable 						=  0;

	if( filename == NULL )
		return -1;

	translation_file = fopen( filename, "r" );
	if( translation_file == NULL )
		return -1;

	while( getline( &cur_line, &cur_len, translation_file ) != -1 )
	{
		sscanf( cur_line, "%s : %c", byte_literal, &readable );

		if( -1 == byte_literal_to_hex_value( byte_hex, byte_literal, 2 * 2 ) )
		{
			delete_byte_to_readable_hash( );
			return -1;
		}

		if( readable == 0 )
			readable = ' ';

		add_byte_to_readable_hash( byte_hex, readable );
	}

	free( cur_line );

	fclose( translation_file );

	return 0;
}

int create_translated_rom( rom_file *rom )
{
	if( rom == NULL || get_byte_to_readable_hash() == NULL )
		return -1;

	print_buffer_contents_f( rom, rom->rom_length );

	delete_byte_to_readable_hash( );

	return 0;
}
