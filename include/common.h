/*!
*	Contains common code used in the application.
*/

/*!
*	rom_file contains the breakdown of a rom to pass around to various functions.	
*
*	rom_path: 		A qualified path to the rom file. Will be used via fopen to open the rom for reading.
*	rom_buffer:		A malloc section of memory that will be created after getting the length of the rom.
*	rom_length:		The length of the rom gotten via fseek
*/
typedef struct {
	char* rom_path;
	unsigned char *rom_buffer;
	unsigned long rom_length;
} rom_file;

/*!
*	dump_file contains the breakdown of a dump file to pass around to various functions.
*
*	dump_path:			A qualified path to the dump file. Will be used via fopen to open the dump for reading.
*	rom_buffer:			A malloc section of memory that will contain the byte representation stored in the dump.
*	rom_length:			The length of the rom section in the dump. Calculated by the amount of lines in the dump * 32 (the amount of bytes per line).
*	translated_buffer:	A malloc section of memory that will contain the text representation stored in the dump.
*	translated_length:	The lenght of the text section in the dump. Calculated by the amount of lines in the dump * 16 (rom_length / 2).
*/
typedef struct {
	char* dump_path;
	unsigned char *rom_buffer;
	unsigned long rom_length;
	unsigned char *translated_buffer;
	unsigned long translated_length;
} dump_file;

/*!
*	An example line from a dump file:
*	0x003DD120 |	800080FF 81007FFF 81007FFF 82007EFF 82007EFF 82007EFF 83007DFF 83007DFF 	|   A A B B B C C 
*/
#define BYTES_PER_DUMP_LINE			32
#define TEXT_PER_DUMP_LINE  		16
#define ADDRESS_PER_DUMP_LINE		11
#define SPACES_PER_BYTE_DUMP_LINE	10
#define BYTES_PER_SEGMENT			4