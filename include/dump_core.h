#ifndef COMMON_H
#define COMMON_H
	#include "common.h"
#endif

#ifndef OUTPUT_H
#define OUTPUT_H
	#include "output.h"
#endif

#ifndef HASHING_H
#define HASHING_H
	#include "hashing.h"
#endif

int get_dump_amount_of_lines( dump_file* );
int read_dump_file( dump_file* );
int write_translated_dump( dump_file*, char*);
