#include <stdio.h>
#include <stdlib.h>

#include "../include/dump_core.h"
#include "../include/utils/utils.h"

/*!
*	Given a dump_file with a valid path, fill in the rom_length and translated length with sizes calculated from the amount of lines in the dump.
*
*	dump:	A pointer to a dump file with dump->dump_path containing a valid path
*
*	Returns 0 on success, -1 if dump path is null or the dump can't be opened.
*/
int get_dump_amount_of_lines( dump_file *dump )
{
	FILE *dump_file 					= NULL;

	char *cur_line						= NULL;
	size_t cur_len 						= 0;

	unsigned long amount_of_lines		= 0;

	if( dump == NULL || dump->dump_path == NULL )
		return -1;

	dump_file = fopen( dump->dump_path, "r" );
	if( dump_file == NULL )
		return -1;

	while( getline( &cur_line, &cur_len, dump_file ) != -1 )
	{
		amount_of_lines++;
	}

	if( cur_line != NULL)
	{
		free( cur_line );
	}

	fclose( dump_file );

	dump->rom_length 			= amount_of_lines * BYTES_PER_DUMP_LINE;
	dump->translated_length 	= amount_of_lines * TEXT_PER_DUMP_LINE;

	return 0;
}

/*!
*	Given a dump file, parse the file into memory.
*
*	dump:	A pointer to a dump file with a valid path, and allocated memory for the rom_buffer and translated_buffer.
*
*	Returns 0 on success, -1 on non-allocated memory or an issue reading the dump_path.
*/
int read_dump_file( dump_file *dump )
{
	FILE *dump_file 																	= NULL;

	size_t cur_len 																		= 0;

	unsigned char address[ ADDRESS_PER_DUMP_LINE ]										= { 0 };
	unsigned char byte_literal[ BYTES_PER_DUMP_LINE * 2 + SPACES_PER_BYTE_DUMP_LINE ] 	= { 0 };
	unsigned char byte_hex[ BYTES_PER_DUMP_LINE ] 										= { 0 };
	unsigned char translated_literal[ TEXT_PER_DUMP_LINE + 2 ] 							= { 0 };	//the extra 2 to account for spaces read in via sscanf

	unsigned long cur_rom_buffer_pos													= 0;

	char *cur_line																		= NULL;

	if( dump == NULL || dump->dump_path == NULL || dump->rom_buffer == NULL || dump->translated_buffer == NULL )
		return -1;	

	dump_file = fopen( dump->dump_path, "r" );
	if( dump_file == NULL )
		return -1;

	while( getline( &cur_line, &cur_len, dump_file ) != -1 )
	{
		//Line format is:  0xDEADBEEF |	2E000000 EA00...	| text... 
		//An example line: 0x003DD120 |	800080FF 81007FFF 81007FFF 82007EFF 82007EFF 82007EFF 83007DFF 83007DFF 	|   A A B B B C C 
		sscanf( cur_line, "%s |\t%71c \t|%17c", address, byte_literal, translated_literal );

		remove_spaces_from_string( byte_literal );

		byte_literal_to_hex_value( byte_hex, (char*)byte_literal, BYTES_PER_DUMP_LINE * 2 );
		memcpy( dump->rom_buffer + cur_rom_buffer_pos, byte_hex, BYTES_PER_DUMP_LINE );

		//first character is a space to deal with sscanf not filling values correctly otherwise
		memcpy( dump->translated_buffer + (cur_rom_buffer_pos / 2 ), translated_literal + sizeof( unsigned char ), TEXT_PER_DUMP_LINE );
		cur_rom_buffer_pos += BYTES_PER_DUMP_LINE;

		memset( translated_literal, 0, TEXT_PER_DUMP_LINE + 2 );
	}

	if( cur_line != NULL )
	{
		free( cur_line );
	}

	fclose( dump_file );

	return 0;
}

/*!
*	Give a dump file, write any changes between the rom and text sections to a new rom. Requires the byte_to_readable hash to be filled.
*
*	dump:	A pointer to a dump file with all of its members filled with data.
*
*	Returns 0 on success, -1 on a null pointer or no hash existing.
*	Upon returning, will delete the hash structure.
*/
int write_translated_dump( dump_file *dump )
{
	char readable 						= 0;

	unsigned char byte_value[ 2 ]		= { 0 };
	unsigned char new_byte_value[ 2 ]	= { 0 };

	if( dump == NULL || get_byte_to_readable_hash() == NULL )
		return -1;

	for( int i = 0; i < dump->rom_length; i += 2 )
	{
		memcpy( byte_value, dump->rom_buffer + i, 2 );

		//if we find the character in the text section in the hash, and it's value doesn't equal the bytes in the rom section, change the rom section
		if( (readable = find_byte_to_readable_hash_value( byte_value ) ) != 0 )
		{
			if( dump->translated_buffer[ i / 2 ] != readable )
			{
				if( 1 == find_readable_to_byte_hash_value( dump->translated_buffer[ i / 2 ], new_byte_value ) )
				{
					memcpy( dump->rom_buffer + i, new_byte_value, 2 );
				}
			}
		}
	}

	fwrite( dump->rom_buffer, dump->rom_length, 1, stdout );

	delete_hashes( );

	return 0;
}

/*!
*	Given a dump file, start and end addresses, dump all the text between start and end into a newline separated list.
*
*	dump:				A pointer to a dump file with all of its members filled with data.
*	start_address:		Start of the memory section to dump.
*	end_address:		End of the memory section to dump. Must be greater than start_address.
*	rom_string_break:	The two-character byte-code the rom uses to distinguish between different text strings.
*
*	Returns 0 on success, -1 on invalid arguments.
*/
int write_dump_strings( dump_file *dump, unsigned long start_address, unsigned long end_address, char* rom_string_break )
{
	unsigned char byte_rom_string_break[ 2 ]			= { 0 };

	if( dump == NULL || start_address > end_address || end_address > dump->rom_length || rom_string_break == NULL )
		return -1;

	byte_literal_to_hex_value( byte_rom_string_break, rom_string_break, 2 * 2 );

	for( unsigned long i = start_address; i < end_address; i += 2 )
	{
		print_newline_or_character( dump->rom_buffer[ i ] == byte_rom_string_break[ 0 ] && dump->rom_buffer[ i + 1 ] == byte_rom_string_break[ 1 ] ,
			dump->translated_buffer[ i / 2 ] );
	}

	return 0;
}

/*!
*	Given a dump file, strings file, start and end addresses, reupload all the strings to the dump's text section.
*
*	dump:				A pointer to a dump file with all of its members filled with data.
*	strings_file_path:	A path to a newline separated section of strings to write back to the dump.
*	start_address:		Start of the memory section to write back to.
*	end_address:		End of the memory section to write back to. Must be greater than start_address.
*	rom_string_break:	The two-character byte-code the rom uses to distinguish between different text strings.		
*
*	Returns 0 on success, -1 on invalid arguments or error reading the strings file.
*/
int read_and_translate_dump_strings( dump_file *dump, char* strings_file_path, unsigned long start_address, 
	unsigned long end_address, char* rom_string_break )
{
	FILE *strings_file 							= NULL;

	unsigned char byte_rom_string_break[ 2 ]	= { 0 };

	char *cur_line								= NULL;

	size_t cur_len 								= 0;
	ssize_t read_len 							= 0;

	unsigned long cur_dump_offset				= 0;

	int current_write_position					= 0;

	if( dump == NULL || strings_file_path == NULL || start_address > end_address ||
		 end_address > dump->rom_length || rom_string_break == NULL )
		return -1;

	byte_literal_to_hex_value( byte_rom_string_break, rom_string_break, 2 * 2 );

	strings_file = fopen( strings_file_path, "r" );
	if( strings_file == NULL )
		return -1;

	//The purpose of these nested loops is to first loop through all the strings in the strings file, 
	//and then loop through the dump, writing characters back until we reach a break. Due to the fact,
	//that we are modifying text pointers in the rom's memory, we need to ensure we keep the size of all elements.
	//This includes padding out strings that are too short when passed in.
	while( (read_len = getline( &cur_line, &cur_len, strings_file ) ) != -1 )
	{
		current_write_position = 0;

		while( !( dump->rom_buffer[ start_address + cur_dump_offset ] == byte_rom_string_break[ 0 ] && 
			dump->rom_buffer[ start_address + cur_dump_offset + 1 ] == byte_rom_string_break[ 1 ]) )
		{
			if( current_write_position > read_len - 2 )
			{
				dump->translated_buffer[ ( start_address + cur_dump_offset ) / 2 ] = ' ';
			}
			else
			{
				dump->translated_buffer[ ( start_address + cur_dump_offset ) / 2 ] = cur_line[ current_write_position++ ];
			}

			cur_dump_offset += 2;
		}

		//we increment again to skip over the break characters that broke the inner while loop.
		cur_dump_offset += 2;
	}

	if( cur_line != NULL )
	{
		free( cur_line );
	}

	fclose( strings_file );

	return 0;
}
