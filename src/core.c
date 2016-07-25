#include <stdio.h>
#include <stdlib.h>

#include "../include/core.h"
#include "../include/utils/utils.h"

/*
* TODO: 
* 	Move all output to output file
* 	Remove magic numbers
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

int create_translation_file( char *filename, int unicode )
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
        print_buffer_as_bytes( translation_file, s->byte_value, unicode ? 2 : 1 );
        fprintf( translation_file, " : %c\n", s->readable );
    }

    fclose( translation_file );

	return 0;
}

int read_translation_file( char *filename, int unicode )
{
	FILE *translation_file 				= NULL;

	char *cur_line						= NULL;
	size_t cur_len 						= 0;

	unsigned char byte_hex[ 2 ]			= { 0 };
	unsigned char byte_literal[ 4 ]		= { 0 };
	char readable 						=  0;

	int step_value						= unicode ? 2 : 1;

	if( filename == NULL )
		return -1;

	translation_file = fopen( filename, "r" );
	if( translation_file == NULL )
		return -1;

	while( getline( &cur_line, &cur_len, translation_file ) != -1 )
	{
		sscanf( cur_line, "%s : %c", byte_literal, &readable );

		if( -1 == byte_literal_to_hex_value( byte_hex, byte_literal, step_value * 2 ) )
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

int create_translated_rom( rom_file *rom, int unicode )
{
	if( rom == NULL || get_byte_to_readable_hash() == NULL )
		return -1;

	print_buffer_contents_f( rom, rom->rom_length, unicode );

	delete_byte_to_readable_hash( );

	return 0;
}

int get_dump_amount_of_lines( dump_file *dump )
{
	FILE *dump_file 					= NULL;

	if( dump == NULL || dump->dump_path == NULL )
		return -1;

	dump_file = fopen( dump->dump_path, "r" );
	if( dump_file == NULL )
		return -1;

	char *cur_line						= NULL;
	size_t cur_len 						= 0;

	unsigned long amount_of_lines		= 0;

	while( getline( &cur_line, &cur_len, dump_file ) != -1 )
	{
		amount_of_lines++;
	}

	free( cur_line );

	fclose( dump_file );

	dump->rom_length 			= amount_of_lines * 32;
	dump->translated_length 	= amount_of_lines * 16;

	return 0;
}

int read_dump_file( dump_file *dump )
{
	FILE *dump_file 						= NULL;

	if( dump == NULL || dump->dump_path == NULL || dump->rom_buffer == NULL || dump->translated_buffer == NULL )
		return -1;	

	dump_file = fopen( dump->dump_path, "r" );
	if( dump_file == NULL )
		return -1;

	char *cur_line							= NULL;
	size_t cur_len 							= 0;

	unsigned char address[ 11 ]				= { 0 };

	unsigned char byte_literal[ 72 ] 		= { 0 };
	unsigned char byte_hex[ 32 ] 			= { 0 };

	unsigned char translated_literal[ 17 ] 	= { 0 };

	unsigned long cur_rom_buffer_pos		= 0;

	while( getline( &cur_line, &cur_len, dump_file ) != -1 )
	{
		sscanf( cur_line, "%s |\t%71c \t| %16c", address, byte_literal, translated_literal );

		remove_spaces_from_string( byte_literal );

		byte_literal_to_hex_value( byte_hex, byte_literal, 64 );
		memcpy( dump->rom_buffer + cur_rom_buffer_pos, byte_hex, 32 );
		memcpy( dump->translated_buffer + (cur_rom_buffer_pos / 2 ), translated_literal, 16 );
		cur_rom_buffer_pos += 32;
	}

	free( cur_line );

	fclose( dump_file );

	return 0;
}
