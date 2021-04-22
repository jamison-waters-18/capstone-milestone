#ifndef ABILITY_H
#define ABILITY_H

#include <glib.h>
#include "Dyna.h"

typedef struct {
	char* private_name;
	char* public_name;
	char* source_book;
	char* prereqs;
	char* sequence;
} AbilityDef;

generate_dyna_headers_M(AbilityDef);

AbilityDef* ability_def_init(AbilityDef*);
int ability_def_clear(AbilityDef*);

typedef struct {
	char* private_name;
	int number;
} AbilityRef;

generate_dyna_headers_M(AbilityRef);

AbilityRef* ability_ref_init(AbilityRef*);
int ability_ref_clear(AbilityRef*);

#endif
