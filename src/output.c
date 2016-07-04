#include <stdio.h>

#include "../include/output.h"

void print_buffer_contents_f( rom_file *rom, unsigned long len_to_display )
{
	if( rom == NULL || rom->rom_buffer == NULL )
		return;

	if( len_to_display == 0 )
		len_to_display = rom->rom_length;

	for ( int i = 0; i < len_to_display; i++ )
	{
	     printf( "%.2X", rom->rom_buffer[ i ] );

	     if (i % 4 == 3)
	         printf(" ");

	     if (i % 32 == 31)
	         printf("\n");
	}	

	printf("\n");
}

void print_match_list( rom_file *rom, match_info *matches, 
	int relative_search_text_length, int unicode_flag) 
{
	if( rom == NULL || rom->rom_buffer == NULL || matches == NULL )
		return;

	if( matches->amount_of_matches == 0 )
	{
		printf("No matches found. Try adding fuzz or switching encodings. (or your text might actually be a graphic!)\n");
		return;
	}

	for( int i = 0; i < matches->amount_of_matches; i++ )
	{
		printf("0x%.8lX\t", matches->location_matches[ i ]);
		for( int c = 0; c < relative_search_text_length * (unicode_flag ? 2 : 1); c++ )
		{
			printf("%.2X", rom->rom_buffer[ matches->location_matches[ i ] + c ]);
		}

		printf("\n");
	}

}