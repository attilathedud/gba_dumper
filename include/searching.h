#ifndef COMMON_H
#define COMMON_H
	#include "common.h"
#endif

#define MAX_LOCATION_MATCHES 		25

/*!
*	Used for holding information about the match set to pass to the generate_translation_set_from_matches() function.
*	location_matches will hold the offset of the matches in the rom buffer, not the actual matches themselves.
*/
typedef struct {
	int amount_of_matches;
	unsigned long location_matches[ MAX_LOCATION_MATCHES ];
} match_info;

/*!
*	Given a rom and string to search for, fill the match_info pointer with possible matches.
*/
int relative_search( rom_file*, match_info*, char*, int );
