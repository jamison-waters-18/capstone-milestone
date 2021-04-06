#include <string.h>
#include "../includes/character.h"
#include "../includes/glib-facade.h"
#include "../includes/DynaImp.h"

generate_dyna_functions_M(Process);

Process* process_init(Process* p) {
	if (!p) return NULL;
	p->private_name = NULL;
	p->sequence = NULL;
	p->when_to_run = LONG_REST;
	return p;
}

int process_clear(Process* p) {
	if (!p) return -1;
	free(p->private_name);
	free(p->sequence);
	return 0;
}

Act* act_init(Act* a) {
	if (!a) return NULL;
	a->duration = ACTION;
	a->private_name = NULL;
	a->public_name = NULL;
	a->prereqs = NULL;
	a->sequence = NULL;
	return a;
}

int act_clear(Act* a) {
	if (!a) return -1;
	free(a->private_name);
	free(a->public_name);
	free(a->prereqs);
	free(a->sequence);
	return 0;
}

generate_dyna_functions_M(Act);

ArbitraryState* arbitrary_state_init(ArbitraryState*a) {
	if (!a) return NULL;
	a->private_name = NULL;
	a->sequence = NULL;
	return a;
}

int arbitrary_state_clear(ArbitraryState* a) {
	if (!a) return -1;
	free(a->private_name);
	free(a->sequence);
	return 0;
}

generate_dyna_functions_M(ArbitraryState);

PlayerCharacter* player_character_init(PlayerCharacter* crctr) {
	if (!crctr) { return NULL; }
	crctr->private_name = NULL;
	crctr->public_name = NULL;
	crctr->xp = 0;
	crctr->race = NULL;
	crctr->subrace = NULL;
	//GPtrArray* classes;////////////////////////////////////////////////////////////////////////////
	crctr->hp = 0;
	crctr->chosen_hp_max = 0;
	crctr->thp = 0;
	memset(&(crctr->hit_dice), 0, sizeof(crctr->hit_dice));
	crctr->inspiration_die = 0;
	crctr->valor_inspiration = 0;
	crctr->size = MEDIUM;
	crctr->alignment = UNALIGNED;
	item_ref_init(&(crctr->armor));
	item_ref_init(&(crctr->main_hand));
	item_ref_init(&(crctr->off_hand));
	ItemRefDyna_init(&(crctr->inventory));
	ItemRefDyna_init(&(crctr->on_ground));
	crctr->ac_bonus = 0;
	crctr->verbal_limits = 0;
	crctr->somatic_limits = 0;
	crctr->concentration_limits = 0;
	crctr->walk_speed = 0;
	memset(&(crctr->ability_scores), 0, sizeof(crctr->ability_scores));
	memset(&(crctr->immunities), 0, sizeof(crctr->immunities));
	memset(&(crctr->resistances), 0, sizeof(crctr->resistances));
	crctr->weak_to_silver = 0;
	crctr->weak_to_magic = 0;
	memset(&(crctr->vulnerables), 0, sizeof(crctr->hit_dice));
	crctr->passive_perception_bonus = 0;
	memset(&(crctr->skill_half_proficiencies), 0, sizeof(crctr->skill_half_proficiencies));
	memset(&(crctr->skill_proficiencies), 0, sizeof(crctr->skill_proficiencies));
	memset(&(crctr->skill_expertise), 0, sizeof(crctr->skill_expertise));
	crctr->exhaustion = 0;
	crctr->languages = g_ptr_array_new();
	AbilityRefDyna_init(&(crctr->abilities));
	AbilityRefDyna_init(&(crctr->inactive_abilities));
	memset(&(crctr->spell_slots_max), 0, sizeof(crctr->spell_slots_max));
	memset(&(crctr->spell_slots), 0, sizeof(crctr->spell_slots));
	memset(&(crctr->status_effects), 0, sizeof(crctr->status_effects));
	ArbitraryStateDyna_init(&(crctr->arbitrary_states));
	crctr->arbitrary_counter = my_g_str_int_hash_table_new();
	ProcessDyna_init(&(crctr->processes));
	NoteDyna_init(&(crctr->notes));
	ActDyna_init(&(crctr->acts));

	Act a;
	act_init(&a);

	/*
	a.duration = ACTION;
	a.private_name = set_string(NULL, "attack");
	a.public_name = set_string(NULL, "Attack");
	ActDyna_add(&(crctr->acts), a); */

	a.duration = ACTION;
	a.private_name = set_string(NULL, "dash");
	a.public_name = set_string(NULL, "Dash");
	ActDyna_add(&(crctr->acts), a);

	a.private_name = set_string(NULL, "disengage");
	a.public_name = set_string(NULL, "Disengage");
	ActDyna_add(&(crctr->acts), a);

	a.private_name = set_string(NULL, "dodge");
	a.public_name = set_string(NULL, "Dodge");
	ActDyna_add(&(crctr->acts), a);

	a.private_name = set_string(NULL, "help");
	a.public_name = set_string(NULL, "Help");
	ActDyna_add(&(crctr->acts), a);

	a.private_name = set_string(NULL, "hide");
	a.public_name = set_string(NULL, "Hide");
	ActDyna_add(&(crctr->acts), a);

	/*
	a.private_name = set_string(NULL, "");
	a.public_name = set_string(NULL, "");
	ActDyna_add(&(crctr->acts), a);

	a.private_name = set_string(NULL, "");
	a.public_name = set_string(NULL, "");
	ActDyna_add(&(crctr->acts), a); */

	return crctr;
}

int player_character_clear(PlayerCharacter* crctr) {
	if (!crctr) { return -1; }
	free(crctr->private_name);
	free(crctr->public_name);
	free(crctr->race);
	free(crctr->subrace);
	//GPtrArray* classes;////////////////////////////////////////////////////////////////////////////
	item_ref_clear(&(crctr->armor));
	item_ref_clear(&(crctr->main_hand));
	item_ref_clear(&(crctr->off_hand));
	ItemRefDyna_clear(&(crctr->inventory), item_ref_clear);
	ItemRefDyna_clear(&(crctr->on_ground), item_ref_clear);
	my_dynamic_ptr_array_free(crctr->languages, NULL);
	AbilityRefDyna_clear(&(crctr->abilities), ability_ref_clear);
	AbilityRefDyna_clear(&(crctr->inactive_abilities), ability_ref_clear);
	ArbitraryStateDyna_clear(&(crctr->arbitrary_states), arbitrary_state_clear);
	g_hash_table_destroy(crctr->arbitrary_counter);
	ProcessDyna_clear(&(crctr->processes), process_clear);
	NoteDyna_clear(&(crctr->notes), note_clear);
	ActDyna_clear(&(crctr->acts), act_clear);
	return 0;
}
