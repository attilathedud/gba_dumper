/*!
*	Utility functions used in various places that have no strong ties to the application.
*/

/*!
*	Given a char* containing the literal byte values (e.g., "2E00") will fill the unsigned char* with the actual hex value (e.g., 0x2E00).	
*/
int byte_literal_to_hex_value( unsigned char *, char *, int );

/*!
*	Given a char*, remove spaces over the entire string. Modifies the string in place. 
*/
void remove_spaces_from_string( char * );
