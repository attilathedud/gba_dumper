#include <stdio.h>
#include <stdlib.h>

#include "../include/core.h"

unsigned long get_rom_length( char *rom_path )
{
	FILE *rom_file						= NULL;
	unsigned long rom_length	 		= 0;

	if( rom_path == NULL )
		return -1;

	rom_file = fopen( rom_path, "rb" );
	if( rom_file == NULL )
		return -1;

	fseek( rom_file, 0, SEEK_END );
	rom_length = ftell( rom_file );
	fseek( rom_file, 0, SEEK_SET );

	fclose( rom_file );

	return rom_length;
}

int dump_rom_into_buffer( char *rom_path, unsigned char *buffer, unsigned long rom_length )
{
	FILE *rom_file						= NULL;

	if( rom_path == NULL || buffer == NULL )
		return -1;

	rom_file = fopen( rom_path, "rb" );
	if( rom_file == NULL )
		return -1;

	fread( buffer, rom_length, 1, rom_file );

	fclose( rom_file );

	return 0;
}

void print_buffer_contents_f( unsigned char* buffer, unsigned long len_to_display )
{
	for ( int i = 0; i < len_to_display; i++ )
	{
	     printf( "%.2X", buffer[ i ] );

	     if (i % 4 == 3)
	         printf(" ");

	     if (i % 32 == 31)
	         printf("\n");
	}	

	printf("\n");
}
