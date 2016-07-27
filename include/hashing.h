#pragma GCC diagnostic ignored "-Wpointer-sign"

#include "utils/uthash.h"

typedef struct {
	unsigned char byte_value[ 2 ];
	char readable;
	UT_hash_handle hh;
} byte_to_readable_set;

unsigned int get_byte_to_readable_hash_count( );
char find_byte_to_readable_hash_value( unsigned char* );
int find_readable_to_byte_hash_value( char, unsigned char* );
void sort_byte_to_readable( );
void add_byte_to_readable_hash( unsigned char*, char );
void delete_byte_to_readable_hash( );

byte_to_readable_set* get_byte_to_readable_hash( );