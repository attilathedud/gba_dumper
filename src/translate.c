#include <string.h>
#include <stdlib.h>

#include "../include/translate.h"

/*
* TODO:
*	Allow translation ranges to be specified
*
*/

void generate_translation_range( byte_to_readable_set *s, char start_character, char end_character, int step_value )
{
	unsigned char byte_value[ 2 ] 		= { 0 };

	if( s == NULL )
		return;

	if( s->readable >= start_character && s->readable <= end_character)
	{
		for( int j = 0; j <= s->readable - start_character; j++ )
		{
			memcpy( byte_value, s->byte_value, step_value);
			byte_value[ 0 ]-= j;
			add_byte_to_readable_hash( byte_value, s->readable - j);
		}

		for( int j = 0; j <= end_character - s->readable; j++ )
		{
			memcpy( byte_value, s->byte_value, step_value);
			byte_value[ 0 ]+= j;
			add_byte_to_readable_hash( byte_value, s->readable + j);
		}
	}
}

int generate_translation_set_from_matches( rom_file *rom, char* translate_file_path, match_info *matches, 
	char* relative_search_text, int unicode )
{
	int difference_flag 				= 0;

	int step_value 						= unicode ? 2 : 1;

	unsigned char *data1				= (unsigned char*) malloc( strlen( relative_search_text ) * step_value + 1 );
	unsigned char *data2				= (unsigned char*) malloc( strlen( relative_search_text ) * step_value + 1 );

	unsigned char byte_value[ 2 ] 		= { 0 };

	if( rom == NULL || rom->rom_buffer == NULL || matches== NULL || relative_search_text == NULL )
	{
		free( data1 );
		free( data2 );
		return -1;
	}

	//ensure we have data if there is only one match
	if( matches->amount_of_matches == 1 )
		memcpy(data1, rom->rom_buffer + matches->location_matches[ 0 ], strlen( relative_search_text ) * step_value + 1 );

	//iterate through our matches, ensuring that the data they point to is consistent.
	for( int i = 0; i < matches->amount_of_matches - 1; i++ )
	{
		memcpy(data1, rom->rom_buffer + matches->location_matches[ i ], strlen( relative_search_text ) * step_value + 1 );
		memcpy(data2, rom->rom_buffer + matches->location_matches[ i + 1 ], strlen( relative_search_text ) * step_value + 1 );

		if( strcmp( (const char*)data1, (const char*)data2 ) )
		{
			difference_flag = 1;
		}
	}

	if( difference_flag )
	{
		free( data1 );
		free( data2 );
		return -2;
	}

	//add our initial search text with the found values to our hash
	for( int i = 0; i < strlen( relative_search_text ); i++ )
	{
		memcpy( byte_value, data1 + i * step_value, step_value );
		add_byte_to_readable_hash( byte_value, relative_search_text[ i ]);
	}

	sort_byte_to_readable( );

	byte_to_readable_set *s = get_byte_to_readable_hash( );

	//iterate through our initial search text, and generate the rest of the characters based on what's already there.
	for(int i = 0; i < get_byte_to_readable_hash_count( ) && s != NULL; i++) {
		generate_translation_range( s, 'A', 'Z', step_value );
		generate_translation_range( s, 'a', 'z', step_value );

		s = s->hh.next;
    }
	
	sort_byte_to_readable( );

	int successful_write_flag = create_translation_file( translate_file_path, unicode );

	delete_byte_to_readable_hash( );

	free( data1 );
	free( data2 );
	return successful_write_flag;
}
