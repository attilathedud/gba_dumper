#include <string.h>

#include "../include/output.h"

void print_buffer_contents_f( rom_file *rom, unsigned long len_to_display )
{
	char readable 						= 0;
	unsigned char byte_value[ 2 ]		= { 0 };

	char temp_translated_buffer[ 17 ] 	= { 0 };
	int cur_translated_buffer_pos		= 0;

	int is_translating = (get_byte_to_readable_hash() != NULL);

	if( rom == NULL || rom->rom_buffer == NULL )
		return;

	if( len_to_display == 0 )
		len_to_display = rom->rom_length;

	printf( "0x00000000 |\t" );

	for ( int i = 0; i < len_to_display; i++ )
	{
		printf( "%.2X", rom->rom_buffer[ i ] );

		if( is_translating && i % 2 == 0 )
		{
			memcpy( byte_value, rom->rom_buffer + i, 2 );

			if( (readable = find_byte_to_readable_hash_value( byte_value ) ) != 0 )
			{
				temp_translated_buffer[ cur_translated_buffer_pos++ ] = readable;
			}
			else
			{
				temp_translated_buffer[ cur_translated_buffer_pos++ ] = ' ';
			}
		}

		if (i % 4 == 3)
			printf( " " );

		if (i % 32 == 31)
		{
			if( is_translating )
			{
				printf( "\t| " );

				temp_translated_buffer[ 16 ] = '\0';
				printf( "%s", temp_translated_buffer );
				cur_translated_buffer_pos = 0;
			}

			if( i + 1 < len_to_display )
				printf( "\n0x%.8X |\t", i + 1 );
		}
	}	

	printf("\n");
}

void print_match_list( rom_file *rom, match_info *matches, int relative_search_text_length ) 
{
	if( rom == NULL || rom->rom_buffer == NULL || matches == NULL )
		return;

	if( matches->amount_of_matches == 0 )
	{
		fprintf( stderr, "No matches found. Try adding fuzz - or your text might actually be a graphic.\n" );
		return;
	}

	for( int i = 0; i < matches->amount_of_matches; i++ )
	{
		printf("0x%.8lX\t", matches->location_matches[ i ]);
		for( int c = 0; c < relative_search_text_length * 2; c++ )
		{
			printf("%.2X", rom->rom_buffer[ matches->location_matches[ i ] + c ]);
		}

		printf("\n");
	}
}

void print_buffer_as_bytes( FILE *output_file, unsigned char* buffer, long buffer_length ) 
{
	for( int i = 0; i < buffer_length; i++ )
	{
		output_file == NULL ? printf( "%.2X", buffer[ i ] ) : fprintf( output_file, "%.2X", buffer[ i ] );
	}
}

void print_character_translation( FILE *output_file, char readable )
{
	output_file == NULL ? printf( " : %c\n", readable ) : fprintf( output_file, " : %c\n", readable );
}
