#include <string.h>

#include "../include/searching.h"

/*!
*	Given a rom and string to search for, fill the match_info pointer with possible matches.
*
*	Because we have to scan every byte in the rom and iterate over the text_to_search's length to check for differences,
*	this function runs in O(n^2). However, since the inner loop is limited by text_to_search's length, it's effectively O(n).
*
*	Because we aren't sure of the possible encodings, and we allow fuzz, we can't skip sections when scanning.
*
*	rom:			A pointer to a rom file with its members populated.
*	matches:		A pointer to an allocated match_info structure.
*	text_to_search:	The text to search for.
*	fuzz:			The acceptable amount of error.
*
*	Returns amount of matches on success, -1 on invalid arguments.
*/
int relative_search( rom_file *rom, match_info *matches, char* text_to_search, int fuzz )
{
	if( rom == NULL || rom->rom_buffer == NULL || matches == NULL || text_to_search == NULL )
		return -1;

	for( int i = 0; i < rom->rom_length - strlen( text_to_search ); i++ )
	{
		if( matches->amount_of_matches > MAX_LOCATION_MATCHES - 1 )
			break;

		int match_score = 0;

		//Iterate over each letter of text_to_search, checking whether or not the distance between the letters matches
		//the distance between the bytes.
		for( int c = 0; c < strlen( text_to_search) - 1; c++ )
		{
			if( rom->rom_buffer[ i + ( c * 2 ) ] + ( text_to_search[ c + 1 ] - text_to_search[ c ] ) == 
					rom->rom_buffer[ i + ( c * 2 ) + 2 ] )
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
