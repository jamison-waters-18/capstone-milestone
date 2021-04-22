#include <string.h>
#include <stdio.h>
#include "../includes/character.h"
#include "../includes/parser.h"
#include "../includes/glib-facade.h"
#include "../includes/DynaImp.h"

generate_dyna_functions_M(Note);

Note* note_init(Note* note) {
	if (!note) return NULL;
	note->menu = MAIN_SCREEN;
	note->private_name = NULL;
	note->prereqs = NULL;
	note->message = NULL;
	return note;
}

int note_clear(Note* note) {
	if (!note) return -1;
	free(note->private_name);
	free(note->prereqs);
	free(note->message);
	note_init(note);
	return 0;
}

Process* process_init(Process* p) {
	if (!p) return NULL;
	p->private_name = NULL;
	p->description = NULL;
	p->prereqs = NULL;	
	p->sequence = NULL;	
	p->when_to_run = LONG_REST;
	return p;
}

int process_clear(Process* p) {
	if (!p) return -1;
	free(p->private_name);
	free(p->description);
	free(p->prereqs);
	free(p->sequence);
	process_init(p);
	return 0;
}

generate_dyna_functions_M(Process);

ClassRole* class_role_init(ClassRole* c) {
	if (!c) return NULL;
	c->private_name = NULL;
	c->public_name = NULL;
	c->subclass = NULL;
	c->given_hp_per_level = NULL;
	c->level = 0;
	c->hit_die_size = 0;
	return c;
}

int class_role_clear(ClassRole* c) {
	if (!c) return -1;
	free(c->private_name);
	free(c->public_name);
	free(c->subclass);
	free(c->given_hp_per_level);
	class_role_init(c);
	return 0;
}

generate_dyna_functions_M(ClassRole);

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
	act_init(a);
	return 0;
}

generate_dyna_functions_M(Act);

ArbitraryState* arbitrary_state_init(ArbitraryState* a) {
	if (!a) return NULL;
	a->private_name = NULL;
	a->sequence = NULL;
	return a;
}

int arbitrary_state_clear(ArbitraryState* a) {
	if (!a) return -1;
	free(a->private_name);
	free(a->sequence);
	arbitrary_state_init(a);
	return 0;
}

generate_dyna_functions_M(ArbitraryState);


generate_dyna_functions_M(ItemCategory);

PlayerCharacter* player_character_init(PlayerCharacter* crctr) {
	if (!crctr) { return NULL; }
	crctr->private_name = NULL;
	crctr->public_name = NULL;
	crctr->xp = 0;
	crctr->race = NULL;
	crctr->subrace = NULL;
	ClassRoleDyna_init(&(crctr->classes));
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
	crctr->item_proficiencies = g_ptr_array_new();
	ItemCategoryDyna_init(&(crctr->item_category_proficiencies));
	crctr->exhaustion = 0;
	crctr->languages = g_ptr_array_new();
	AbilityRefDyna_init(&(crctr->active_abilities));
	AbilityRefDyna_init(&(crctr->dormant_abilities));
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

	a.duration = ACTION;
	a.private_name = set_string(NULL, "attack");
	a.public_name = set_string(NULL, "Attack");
	a.prereqs = set_string(NULL, "true");
	a.sequence = set_string(NULL, "run_attack();");
	ActDyna_add(&(crctr->acts), a);
	a.sequence = NULL;

	a.private_name = set_string(NULL, "dash");
	a.public_name = set_string(NULL, "Dash");
	a.prereqs = set_string(NULL, "true");
	ActDyna_add(&(crctr->acts), a);

	a.private_name = set_string(NULL, "disengage");
	a.public_name = set_string(NULL, "Disengage");
	a.prereqs = set_string(NULL, "true");
	ActDyna_add(&(crctr->acts), a);

	a.private_name = set_string(NULL, "dodge");
	a.public_name = set_string(NULL, "Dodge");
	a.prereqs = set_string(NULL, "true");
	ActDyna_add(&(crctr->acts), a);

	a.private_name = set_string(NULL, "help");
	a.public_name = set_string(NULL, "Help");
	a.prereqs = set_string(NULL, "true");
	ActDyna_add(&(crctr->acts), a);

	a.private_name = set_string(NULL, "hide");
	a.public_name = set_string(NULL, "Hide");
	a.prereqs = set_string(NULL, "true");
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
	ClassRoleDyna_clear(&(crctr->classes), class_role_clear);
	item_ref_clear(&(crctr->armor));
	item_ref_clear(&(crctr->main_hand));
	item_ref_clear(&(crctr->off_hand));
	ItemRefDyna_clear(&(crctr->inventory), item_ref_clear);
	ItemRefDyna_clear(&(crctr->on_ground), item_ref_clear);
	my_dynamic_ptr_array_free(crctr->item_proficiencies, NULL);
	ItemCategoryDyna_clear(&(crctr->item_category_proficiencies), NULL);
	my_dynamic_ptr_array_free(crctr->languages, NULL);
	AbilityRefDyna_clear(&(crctr->active_abilities), ability_ref_clear);
	AbilityRefDyna_clear(&(crctr->dormant_abilities), ability_ref_clear);
	ArbitraryStateDyna_clear(&(crctr->arbitrary_states), arbitrary_state_clear);
	g_hash_table_destroy(crctr->arbitrary_counter);
	ProcessDyna_clear(&(crctr->processes), process_clear);
	NoteDyna_clear(&(crctr->notes), note_clear);
	ActDyna_clear(&(crctr->acts), act_clear);
	return 0;
}

int player_max_hp(PlayerCharacter* p) {
	if (!p) return -1;
	int level = xp_to_level(p->xp);
	int conmod = ability_score_to_mod(p->ability_scores[2]);
	int selected_bonus = p->chosen_hp_max;
	//TODO: Do something with class-specific health bonus, allow numbers plus ability mods or whatever
	return (level * conmod) + selected_bonus;
}

Dice player_max_hit_dice(PlayerCharacter* p) {
	Dice result = { { 0, 0, 0, 0, 0, 0, 0, 0 } };
	if (!p) return result;
	for (int i = 0; i < p->classes.size; i++) {
		result.die[dicenum_to_index(p->classes.array[i].hit_die_size)] += p->classes.array[i].level;
	}
	return result;
}

void execute_player_processes(PlayerCharacter* p, processLocationType location) {
	for (int i = 0; i < p->processes.size; i++) {
		if (p->processes.array[i].when_to_run == location) {
			returnType r;
			gboolean* meets_prereqs = eval_expr(p->processes.array[i].    prereqs, &r);
			if (r != RTRN_BOOL) {
				safe_free(meets_prereqs);
				break;
			}
			if (*meets_prereqs) {
				parse_sequence(p->processes.array[i].sequence);
			}
			free(meets_prereqs);
		}
	}
}

void print_player_states(PlayerCharacter* p) {
	for (int i = 0; i < p->arbitrary_states.size; i++) {
		printf("    %s\n", p->arbitrary_states.array[i].private_name);
	}
}

void print_player_counters(PlayerCharacter* p) {
	GStrIntHashTableIter itr;
	char* key;
	int* val;
	g_str_int_hash_table_iter_init(&itr, p->arbitrary_counter);
	while (g_str_int_hash_table_iter_next(&itr, &key, &val)) {
		printf("    %s -> %d\n", key, *val);
	}
}
