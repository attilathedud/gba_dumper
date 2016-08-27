#include <string.h>

#include "../include/output.h"

/*!
*	Given a pointer to a rom and its length, print out its contents in a formatted manner.
*
*	If the byte->readable hash is filled, apply a translation while printing.
*
*	rom:			A pointer to a rom_file with all of its members filled with data.
*	len_to_display:	The amount of the buffer to print. If 0, print the whole buffer.
*/
void print_buffer_contents_f( rom_file *rom, unsigned long len_to_display )
{
	char temp_translated_buffer[ TEXT_PER_DUMP_LINE + 1 ] 	= { 0 };
	char readable 											= 0;

	unsigned char byte_value[ 2 ]							= { 0 };
	
	int cur_translated_buffer_pos							= 0;
	int is_translating 										= ( get_byte_to_readable_hash() != NULL );

	if( rom == NULL || rom->rom_buffer == NULL )
		return;

	if( len_to_display == 0 )
		len_to_display = rom->rom_length;

	printf( "0x00000000 |\t" );

	for ( unsigned long i = 0; i < len_to_display; i++ )
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

		if ( i % BYTES_PER_SEGMENT == BYTES_PER_SEGMENT - 1 )
		{
			printf( " " );
		}

		if ( i % BYTES_PER_DUMP_LINE == BYTES_PER_DUMP_LINE - 1 )
		{
			if( is_translating )
			{
				printf( "\t| " );

				temp_translated_buffer[ TEXT_PER_DUMP_LINE ] = '\0';
				printf( "%s", temp_translated_buffer );
				cur_translated_buffer_pos = 0;
			}

			if( i + 1 < len_to_display )
			{
				printf( "\n0x%.8lX |\t", i + 1 );
			}
		}
	}	

	printf("\n");
}

/*!
*	Given a pointer to a dump file and its length, print out its contents in a formatted manner.
*
*	dump:			A pointer to a dump_file with all of its members filled with data.
*	len_to_display:	The amount of the buffer to print. If 0, print the whole buffer.
*/
void reprint_dump_contents( dump_file *dump, unsigned long len_to_display )
{
	unsigned char temp_translated_buffer[ TEXT_PER_DUMP_LINE + 1 ] 	= { 0 };

	if( dump == NULL || dump->rom_buffer == NULL || dump->translated_buffer == NULL )
		return;

	if( len_to_display == 0 )
		len_to_display = dump->rom_length;

	printf( "0x00000000 |\t" );

	for ( unsigned long i = 0; i < len_to_display; i++ )
	{
		printf( "%.2X", dump->rom_buffer[ i ] );

		if ( i % BYTES_PER_SEGMENT == BYTES_PER_SEGMENT - 1 )
		{
			printf( " " );
		}

		if ( i % BYTES_PER_DUMP_LINE == BYTES_PER_DUMP_LINE - 1 )
		{
			printf( "\t| " );

			memcpy( temp_translated_buffer, dump->translated_buffer + ( i - ( BYTES_PER_DUMP_LINE - 1 ) ) / 2, 
				TEXT_PER_DUMP_LINE );
			temp_translated_buffer[ TEXT_PER_DUMP_LINE ] = '\0';
			printf( "%s", temp_translated_buffer );

			if( i + 1 < len_to_display )
			{
				printf( "\n0x%.8lX |\t", i + 1 );
			}
		}
	}	

	printf("\n");
}

/*!
*	Given a pointer to a rom and a set of matches, print out the list of matches and their locations.
*
*	rom:							A pointer to a rom_file with all of its members filled with data.
*	matches:						A pointer to a list of matches returned from relative_search().
*	relative_search_text_length:	Length of the text searched for.
*/
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
		printf( "%d: 0x%.8lX\t", i + 1, matches->location_matches[ i ] );
		for( int c = 0; c < relative_search_text_length * 2; c++ )
		{
			printf( "%.2X", rom->rom_buffer[ matches->location_matches[ i ] + c ] );
		}

		printf( "\n" );
	}
}

/*!
*	Given a buffer, print out its contents formatted as 2 character bytes.
*
*	output_file:	File pointer to output the text to. If null, send to stdout.
*	buffer:			Buffer to output.
*	buffer_length:	Length of the buffer.
*/
void print_buffer_as_bytes( FILE *output_file, unsigned char* buffer, long buffer_length ) 
{
	for( int i = 0; i < buffer_length; i++ )
	{
		output_file == NULL ? printf( "%.2X", buffer[ i ] ) : fprintf( output_file, "%.2X", buffer[ i ] );
	}
}

/*!
*	Given a character, print it in the translation file format.
*
*	output_file:	File pointer to output the text to. If null, send to stdout.
*	readable:		Character to print.
*/
void print_character_translation( FILE *output_file, char readable )
{
	output_file == NULL ? printf( " : %c\n", readable ) : fprintf( output_file, " : %c\n", readable );
}

/*!
*	Given a conditional and character, print a newline if the conditional is true, the character otherwise.
*
*	condition: 			The conditional to resolve.
*	character_to_print:	The character to print.
*/
void print_newline_or_character( int condition, char character_to_print )
{
	if( condition )
		printf( "\n" );
	else
		printf( "%c", character_to_print );
}
