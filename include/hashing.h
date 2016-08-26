#include "utils/uthash.h"

/*!
*	The uthash project (utils/uthash.h) is used to handle hashing for the application. 
*
*	This is just a section of application specific helper method to interface with that library.
*/

/*!
*	Two hashes are used: byte_value->readable and readable->byte_value. Since uthash doesn't care about order of keys in a struct,
*	one struct is used to represent both these hashes.
*/
typedef struct {
	unsigned char byte_value[ 2 ];
	char readable;
	UT_hash_handle hh;
} byte_to_readable_set;

/*!
*	Returns the amount of elements in the byte->readable hash. This can differ from the readable->byte hash if several bytes are mapped to the same character.
*/
unsigned int get_byte_to_readable_hash_count( );

/*!
*	Given a byte value, return the character it is mapped to.
*/
char find_byte_to_readable_hash_value( unsigned char* );

/*!
*	Given a character value, populate a buffer with the byte value it is mapped to.
*/
int find_readable_to_byte_hash_value( char, unsigned char* );

/*!
*	Sort the byte->readable hash alphabetically.
*/
void sort_byte_to_readable( );

/*!
*	Add a character, byte_value pair to both sets of hashes.
*/
void add_element_to_hashes( unsigned char*, char );

/*!
*	Delete both sets of hashes and their contents.
*/
void delete_hashes( );

/*!
*	Return a pointer to the byte->readable hash. Due to the fact that uthash implements hashes via macros, encapsulating 
*	all hash access is borderline impossible. As a result, occasionally we need to directly access the byte->readable hash.
*
*	Only use if absolutely necessary.	
*/
byte_to_readable_set* get_byte_to_readable_hash( );
