#include <string.h>
#include <stdlib.h>

#include "../../include/utils/utils.h"

//Modified form of the answer provided here: 
//https://stackoverflow.com/questions/3408706/hexadecimal-string-to-byte-array-in-c/21510910#21510910
int byte_literal_to_hex_value( unsigned char *byte_hex, char *byte_literal, int byte_literal_len )
{
	char *cur_position_in_byte_literal = byte_literal;
	char temp_hex_buffer[ 5 ] = { '0', 'x', 0, 0, 0 };

	char *end_of_literal;

	if( byte_literal[ 0 ] == '\0' || strlen( byte_literal ) % 2 )
		return -1;

	for( int i = 0; i < byte_literal_len; i++ )
	{
		temp_hex_buffer[ 2 ] = cur_position_in_byte_literal[ 0 ];
		temp_hex_buffer[ 3 ] = cur_position_in_byte_literal[ 1 ];

		byte_hex[ i ] = strtol( temp_hex_buffer, &end_of_literal, 0 );
		cur_position_in_byte_literal += 2 * sizeof( char );
	}

	return 0;
}

void remove_spaces_from_string( char *str )
{
	int count = 0;

	for( int i = 0; str[ i ] != '\0'; i++ )
	{
		if( str[ i ] != ' ' )
		{
			str[ count++ ] = str[ i ];
		}
	}

	str[ count ] = '\0';
}