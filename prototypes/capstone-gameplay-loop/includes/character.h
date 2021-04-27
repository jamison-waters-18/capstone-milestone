#ifndef CHARACTER_H
#define CHARACTER_H

#include "utils.h"
#include "item.h"
#include "ability.h"
#include "Dyna.h"
#include "glib-facade.h"

typedef struct {
	menuType menu;
	char* private_name;
	char* prereqs;
	char* message;
} Note;

generate_dyna_headers_M(Note);

Note* note_init(Note*);
int note_clear(Note*);

typedef struct {
	char* private_name;
	char* description;
	char* prereqs;
	char* sequence;
	processLocationType when_to_run;
} Process;

Process* process_init(Process*);
int process_clear(Process*);

generate_dyna_headers_M(Process);

typedef struct {
	char* private_name;
	char* public_name;
	char* subclass;
	char* given_hp_per_level;
	int level;
	int hit_die_size;
} ClassRole;

ClassRole* class_role_init(ClassRole*);
int class_role_clear(ClassRole*);

generate_dyna_headers_M(ClassRole);

typedef struct {
	actType duration;
	char* private_name;
	char* public_name;
	char* prereqs;
	char* sequence;
} Act;

Act* act_init(Act*);

int act_clear(Act* a);

generate_dyna_headers_M(Act);

typedef struct {
	char* private_name;
	char* sequence;
} ArbitraryState;

ArbitraryState* arbitrary_state_init(ArbitraryState*);
int arbitrary_state_clear(ArbitraryState*);

generate_dyna_headers_M(ArbitraryState);

typedef struct {
	itemCategoryType category;
	int flags;
} ItemCategory;

generate_dyna_headers_M(ItemCategory);

typedef struct {
	//simpleCreature* morph;
	char* private_name;
	char* public_name;
	int xp;
	char* race;
	char* subrace;
	ClassRoleDyna classes;
	int hp;
	int chosen_hp_max;
	int thp;
	Dice hit_dice;
	int inspiration_die;
	int valor_inspiration;
	sizeType size;
	alignmentType alignment;
	ItemRef armor;
	ItemRef main_hand;
	ItemRef off_hand;
	ItemRefDyna inventory;
	ItemRefDyna on_ground;
	int ac_bonus;
	int verbal_limits;
	int somatic_limits;
	int concentration_limits;
	int walk_speed; /*
	int swim_speed;
	int climb_speed;
	int fly_speed;
	int long_jump_distance;
	int long_jump_running_start;
	int high_jump_distance; */
	int ability_scores[6];
	int immunities[13];
	int resistances[13];
	int weak_to_silver;
	int weak_to_magic;
	int vulnerables[13];
	int passive_perception_bonus;
	int skill_half_proficiencies[18];
	int skill_proficiencies[18];
	int skill_expertise[18];
	GPtrArray* item_proficiencies;
	ItemCategoryDyna item_category_proficiencies;
	int exhaustion;
	GPtrArray* languages;
	AbilityRefDyna active_abilities;
	AbilityRefDyna dormant_abilities;
	SpellSlots spell_slots_max;
	SpellSlots spell_slots; /*
	SpellList cantrips;
	SpellListDyna spell_lists; */
	int status_effects[15];
	ArbitraryStateDyna arbitrary_states;
	GStrIntHashTable* arbitrary_counter;
	ProcessDyna processes;
	NoteDyna notes;
	ActDyna acts;
} PlayerCharacter;

PlayerCharacter* player_character_init(PlayerCharacter*);
int player_character_clear(PlayerCharacter*);

int player_max_hp(PlayerCharacter*);

int player_ac(PlayerCharacter*);

Dice player_max_hit_dice(PlayerCharacter*);

void execute_player_processes(PlayerCharacter*, processLocationType);

void print_player_states(PlayerCharacter*);

void print_player_counters(PlayerCharacter*);

#endif
