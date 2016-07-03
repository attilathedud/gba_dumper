#include <stdio.h>

void print_buffer_contents_f( unsigned char *buffer, unsigned long len_to_display )
{
	if( buffer == NULL )
		return;

	for ( int i = 0; i < len_to_display; i++ )
	{
	     printf( "%.2X", buffer[ i ] );

	     if (i % 4 == 3)
	         printf(" ");

	     if (i % 32 == 31)
	         printf("\n");
	}	

	printf("\n");
}

void print_match_list( unsigned char *buffer, unsigned long *matches, int match_amount, 
	int relative_search_text_length, int unicode_flag) 
{
	if( buffer == NULL || matches == NULL )
		return;

	if( match_amount == 0 )
	{
		printf("No matches found for search.\n");
		return;
	}

	for( int i = 0; i < match_amount; i++ )
	{
		printf("0x%.8lX\t", matches[ i ]);
		for( int c = 0; c < relative_search_text_length * (unicode_flag ? 2 : 1); c++ )
		{
			printf("%.2X", buffer[ matches[ i ] + c ]);
		}

		printf("\n");
	}

}