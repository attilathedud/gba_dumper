#include "../include/hashing.h"

byte_to_readable_set *byte_to_readable_translation_set = NULL;

void add_byte_to_readable_hash( unsigned char* byte_value, char readable )
{
	byte_to_readable_set *s = NULL;

	HASH_FIND_STR( byte_to_readable_translation_set, byte_value, s);
	if( s == NULL )
	{
		s = malloc( sizeof( byte_to_readable_set ) );
		memcpy( s->byte_value, byte_value, 2 );
		s->readable = readable;
		HASH_ADD_STR( byte_to_readable_translation_set, byte_value, s );
	}
}

unsigned int get_byte_to_readable_hash_count()
{
	return HASH_COUNT( byte_to_readable_translation_set );
}

int byte_to_readable_sort(byte_to_readable_set *a, byte_to_readable_set *b) 
{
	return a->readable - b->readable;
}

void sort_byte_to_readable()
{
	HASH_SORT( byte_to_readable_translation_set, byte_to_readable_sort );
}

void delete_byte_to_readable_hash() {
  byte_to_readable_set *current, *tmp;

  HASH_ITER(hh, byte_to_readable_translation_set, current, tmp) 
  {
  	HASH_DEL( byte_to_readable_translation_set, current ); 
  	free( current );            
  }
}

byte_to_readable_set* get_byte_to_readable_hash( ) 
{
	return byte_to_readable_translation_set;
}
