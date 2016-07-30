#ifndef COMMON_H
#define COMMON_H
	#include "common.h"
#endif

#define MAX_LOCATION_MATCHES 		25

typedef struct {
	int amount_of_matches;
	unsigned long location_matches[ MAX_LOCATION_MATCHES ];
} match_info;

int relative_search( rom_file*, match_info*, char*, int );
