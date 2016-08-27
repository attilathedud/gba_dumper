#ifndef COMMON_H
#define COMMON_H
	#include "common.h"
#endif

#ifndef SEARCHING_H
#define SEARCHING_H
	#include "searching.h"
#endif

#ifndef CORE_H
#define CORE_H
	#include "rom_core.h"
#endif

#ifndef HASHING_H
#define HASHING_H
	#include "hashing.h"
#endif

/*!
*	Given a set of matches, attempt to generate a translation set from them.
*/
int generate_translation_set_from_matches( rom_file*, char*, match_info*, char* );
