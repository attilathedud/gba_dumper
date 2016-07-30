#include <string.h>

#include "../include/searching.h"

int relative_search( rom_file *rom, match_info *matches, char* text_to_search, int fuzz )
{
	if( rom == NULL || rom->rom_buffer == NULL || matches == NULL || text_to_search == NULL )
		return -1;

	for( int i = 0; i < rom->rom_length - strlen( text_to_search ); i++ )
	{
		if( matches->amount_of_matches > MAX_LOCATION_MATCHES - 1 )
			break;

		int match_score = 0;

		for( int c = 0; c < strlen( text_to_search) - 1; c++ )
		{
			if( rom->rom_buffer[ i + (c * 2)] + ( text_to_search[c + 1] - text_to_search[ c ] ) == 
					rom->rom_buffer[ i + (c * 2) + 2] )
			{
				match_score++;
			}
		}

		if( match_score >= strlen( text_to_search ) - fuzz - 1 )
		{
			matches->location_matches[ matches->amount_of_matches++ ] = i;
		}
	}

	return matches->amount_of_matches;
}
