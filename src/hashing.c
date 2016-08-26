#include "../include/hashing.h"

/*!
*	Represents the byte->readable and readable->byte hash sets. 
*
*	Declared within file scope to make sure they can only be accessed via functions or get_byte_to_readable_hash().
*/
byte_to_readable_set *byte_to_readable_translation_set = NULL;
byte_to_readable_set *readable_to_byte_translation_set = NULL;

/*!
*	Helper function for the hash sorting function.
*/
int readable_alphabetical_sort(byte_to_readable_set *a, byte_to_readable_set *b) 
{
	return a->readable - b->readable;
}

/*!
*	Returns the amount of elements in the byte->readable hash. 
*
*	This can differ from the readable->byte hash if several bytes are mapped to the same character.
*/
unsigned int get_byte_to_readable_hash_count()
{
	return HASH_COUNT( byte_to_readable_translation_set );
}

/*!
*	Given a byte value, return the character it is mapped to.
*
*	byte_value:		The byte_value used as the key in byte_value->readable hash.
*
*	Retuns the character matched on success, 0 on no matching character.
*/
char find_byte_to_readable_hash_value( unsigned char* byte_value )
{
	byte_to_readable_set *s = NULL;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-sign"
	HASH_FIND_STR( byte_to_readable_translation_set, byte_value, s );
#pragma GCC diagnostic pop

	if( s == NULL )
		return 0;

	return s->readable;
}

/*!
*	Given a character value, populate a buffer with the byte value it is mapped to.
*
*	readable:		The character used as the key in readable->byte_value.
*	buffer:			A buffer of at least 2 bytes long.
*
*	Returns 1 on success, 0 on no key, and -1 on invalid arguments.
*/
int find_readable_to_byte_hash_value( char readable, unsigned char *buffer )
{
	if( buffer == NULL )
		return -1;

	byte_to_readable_set *s = NULL;

	int _readable = (int)readable;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-sign"
	HASH_FIND_INT( readable_to_byte_translation_set, &_readable, s );
#pragma GCC diagnostic pop

	if( s == NULL )
		return 0;

	memcpy( buffer, s->byte_value, 2 );

	return 1;
}

/*!
*	Add a character, byte_value pair to both sets of hashes.
*
*	Due to the way uthash works, even though we are adding the same elements, memory will be allocated for each pair.
*
*	byte_value:		The byte_value to be added as a key and value.
*	readable:		The character to be added as a key and value.
*/
void add_element_to_hashes( unsigned char* byte_value, char readable )
{
	byte_to_readable_set *s = NULL, *s2 = NULL;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-sign"
	HASH_FIND_STR( byte_to_readable_translation_set, byte_value, s);
#pragma GCC diagnostic pop

	if( s == NULL )
	{
		s = malloc( sizeof( byte_to_readable_set ) );
		memcpy( s->byte_value, byte_value, 2 );
		s->readable = readable;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-sign"
		HASH_ADD_STR( byte_to_readable_translation_set, byte_value, s );
#pragma GCC diagnostic pop

		s2 = malloc( sizeof( byte_to_readable_set ) );
		memcpy( s2->byte_value, byte_value, 2 );
		s2->readable = readable;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-sign"
		HASH_ADD_INT( readable_to_byte_translation_set, readable, s2 );
#pragma GCC diagnostic pop
	}
}

/*!
*	Sort the byte->readable hash alphabetically.
*/
void sort_byte_to_readable()
{
	HASH_SORT( byte_to_readable_translation_set, readable_alphabetical_sort );
}

/*!
*	Delete both sets of hashes and their contents.
*
*	As noted in the add, since we allocate memory for both sets of "identical" content, we need to free both.
*/
void delete_hashes() {
  byte_to_readable_set *current, *tmp;

  HASH_ITER(hh, byte_to_readable_translation_set, current, tmp) 
  {
  	HASH_DEL( byte_to_readable_translation_set, current ); 
  	free( current );            
  }

  HASH_ITER(hh, readable_to_byte_translation_set, current, tmp) 
  {
  	HASH_DEL( readable_to_byte_translation_set, current ); 
  	free( current );            
  }
}

/*!
*	Return a pointer to the byte->readable hash. Due to the fact that uthash implements hashes via macros, encapsulating 
*	all hash access is borderline impossible. As a result, occasionally we need to directly access the byte->readable hash.
*
*	Only use if absolutely necessary.	
*
*	Returns the pointer to the internal byte_to_readable_translation_set member.
*/
byte_to_readable_set* get_byte_to_readable_hash( ) 
{
	return byte_to_readable_translation_set;
}
