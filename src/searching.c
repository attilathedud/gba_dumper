#include <string.h>

#include "../include/searching.h"

int relative_search( unsigned char *rom_buffer, unsigned long rom_length, unsigned long *location_matches, 
	int location_matches_length, char* text_to_search, int unicode, int fuzz )
{
	int amount_of_matches = 0;

	if( rom_buffer == NULL || location_matches == NULL || text_to_search == NULL )
		return -1;

	//if unicode, assume 2 bytes for each letter, so step ever other byte while searching
	int step_value = unicode ? 2 : 1;

	for( int i = 0; i < rom_length - strlen( text_to_search ); i++ )
	{
		if( amount_of_matches > location_matches_length - 1 )
			break;

		int match_score = 0;

		for( int c = 0; c < strlen( text_to_search) - 1; c++ )
		{
			if( rom_buffer[ i + (c * step_value)] + ( text_to_search[c + 1] - text_to_search[ c ] ) == 
					rom_buffer[ i + (c * step_value) + step_value] )
			{
				match_score++;
			}
		}

		if( match_score >= strlen( text_to_search ) - fuzz - 1 )
		{
			location_matches[ amount_of_matches++ ] = i;
		}
	}

	return amount_of_matches;
}
