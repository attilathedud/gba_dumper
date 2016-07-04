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
