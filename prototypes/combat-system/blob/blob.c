#include "../includes/blob.h"

Blob* get_blob() {
	static Blob blob;
	return &blob;
}

void blob_init() {
	Blob* b = get_blob();
	ItemDefDyna_init(&(b->item_db));
	player_character_init(&(b->player));
	DamageDyna_init(&(b->damages));
	b->selected_ability = NULL;
	b->is_ranged = 0;
	//b->proper_turn = 0;
}

void blob_clear() {
	Blob* b = get_blob();
	ItemDefDyna_clear(&(b->item_db), item_def_clear);
	player_character_clear(&(b->player));
	ActDyna_clear(&(b->act_stack), act_clear);
	DamageDyna_clear(&(b->damages), NULL);
	free(b->selected_ability);
}
