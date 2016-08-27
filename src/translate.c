#include <string.h>
#include <stdlib.h>

#include "../include/translate.h"


/*!
*	Helper function used by generate_translation_set_from_matches to generate match sets from a-z and A-Z.
*
*	s:					The byte->readable hash containing the tranlsations.
*	start_character:	The start character to translate from.
*	end_character:		The end character to translate to.
*/
void generate_translation_range( byte_to_readable_set *s, char start_character, char end_character )
{
	unsigned char byte_value[ 2 ] 		= { 0 };

	if( s == NULL )
		return;

	if( s->readable >= start_character && s->readable <= end_character)
	{
		for( int j = 0; j <= s->readable - start_character; j++ )
		{
			memcpy( byte_value, s->byte_value, 2 );
			byte_value[ 0 ] -= j;
			add_element_to_hashes( byte_value, s->readable - j );
		}

		for( int j = 0; j <= end_character - s->readable; j++ )
		{
			memcpy( byte_value, s->byte_value, 2 );
			byte_value[ 0 ] += j;
			add_element_to_hashes( byte_value, s->readable + j );
		}
	}
}

/*!
*	Given a set of matches, attempt to generate a translation set from them.
*
*	rom:					A pointer to a rom file with populated members.
*	translate_file_path:	Path to output the translation file to.
*	matches:				A pointer to a populated set of matches.
*	relative_search_text:	The text our match set was generated from.
*
*	Create a translation file, delete the hashes, and return the value of the write on success, -1 on failure, and -2 on
*	conflicting match sets.
*/
int generate_translation_set_from_matches( rom_file *rom, char* translate_file_path, match_info *matches, 
	char* relative_search_text)
{
	int difference_flag 				= 0;

	unsigned char *data1				= (unsigned char*) malloc( strlen( relative_search_text ) * 2 + 1 );
	unsigned char *data2				= (unsigned char*) malloc( strlen( relative_search_text ) * 2 + 1 );

	unsigned char byte_value[ 2 ] 		= { 0 };

	if( rom == NULL || rom->rom_buffer == NULL || matches== NULL || relative_search_text == NULL )
	{
		free( data1 );
		free( data2 );
		return -1;
	}

	//Ensure we have data if there is only one match
	if( matches->amount_of_matches == 1 )
		memcpy(data1, rom->rom_buffer + matches->location_matches[ 0 ], strlen( relative_search_text ) * 2 + 1 );

	//Iterate through our matches, ensuring that the data they point to is consistent.
	for( int i = 0; i < matches->amount_of_matches - 1; i++ )
	{
		memcpy(data1, rom->rom_buffer + matches->location_matches[ i ], strlen( relative_search_text ) * 2 + 1 );
		memcpy(data2, rom->rom_buffer + matches->location_matches[ i + 1 ], strlen( relative_search_text ) * 2 + 1 );

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

	//Add our initial search text with the found values to our hash
	for( int i = 0; i < strlen( relative_search_text ); i++ )
	{
		memcpy( byte_value, data1 + i * 2, 2 );
		add_element_to_hashes( byte_value, relative_search_text[ i ]);
	}

	sort_byte_to_readable( );

	byte_to_readable_set *s = get_byte_to_readable_hash( );

	//Iterate through our initial search text, and generate the rest of the characters based on what's already there.
	//We have to use a for loop or else we will get stuck in an infinite loop as hashes keep getting added.
	for(int i = 0; i < get_byte_to_readable_hash_count( ) && s != NULL; i++) {
		generate_translation_range( s, 'A', 'Z' );
		generate_translation_range( s, 'a', 'z' );

		s = s->hh.next;
    }
	
	sort_byte_to_readable( );

	int successful_write_flag = create_translation_file( translate_file_path );

	delete_hashes( );

	free( data1 );
	free( data2 );
	return successful_write_flag;
}
