typedef struct {
	char* rom_path;
	unsigned char *rom_buffer;
	unsigned long rom_length;
} rom_file;

typedef struct {
	char* dump_path;
	unsigned char *rom_buffer;
	unsigned long rom_length;
	unsigned char *translated_buffer;
	unsigned long translated_length;
} dump_file;
