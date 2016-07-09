#include <stdio.h>
#include <stdlib.h>

#include "../include/core.h"

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

int create_translation_file( char *filename, byte_to_readable_set *byte_to_readable_translation_set, int unicode )
{
	FILE *translation_file 				= NULL;

	byte_to_readable_set *s 			= NULL;

	if( filename == NULL || byte_to_readable_translation_set == NULL )
		return -1;

	translation_file = fopen( filename, "w" );
	if( translation_file == NULL )
		return -1;

	for( s = byte_to_readable_translation_set; s != NULL; s = s->hh.next ) 
	{
        print_buffer_as_bytes( translation_file, s->byte_value, unicode ? 2 : 1 );
        fprintf( translation_file, ": %c\n", s->readable );
    }

    fclose( translation_file );

	return 0;
}
