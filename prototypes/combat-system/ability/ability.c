#include "../includes/ability.h"
#include "../includes/DynaImp.h"
#include "../includes/glib-facade.h"


generate_dyna_functions_M(AbilityDef);

AbilityDef* ability_def_init(AbilityDef* ablt) {
	if (!ablt) { return NULL; }
	ablt->private_name = NULL;
	ablt->public_name = NULL;
	char book_name[] = "players-handbook";
	ablt->source_book = malloc(strlen(book_name) + 1);
	strcpy(ablt->source_book, book_name);
	ablt->prereqs = NULL;
	ablt->sequence = NULL;
	return ablt;
}

int ability_def_clear(AbilityDef* ablt) {
	if (!ablt) { return -1; }
	free(ablt->private_name);
	free(ablt->public_name);
	free(ablt->source_book);
	free(ablt->prereqs);
	free(ablt->sequence);
	return 0;
}

generate_dyna_functions_M(AbilityRef);

AbilityRef* ability_ref_init(AbilityRef* ablt) {
	if (!ablt) { return NULL; }
	ablt->private_name = NULL;
	return ablt;
}

int ability_ref_clear(AbilityRef* ablt) {
	if (!ablt) { return -1; }
	free(ablt->private_name);
	return 0;
}
