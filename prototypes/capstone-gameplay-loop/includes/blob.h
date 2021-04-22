#ifndef BLOB_H
#define BLOB_H

#include "item.h"
#include "character.h"
#include "ability.h"

typedef struct {
	char* private_name;
	char* public_name;
	char* undo_sequence;
	actType act_type;
} ActRecord;

ActRecord* act_record_init(ActRecord*);
int act_record_clear(ActRecord*);

int act_record_add_undo_command(char*);

int act_record_undo();

generate_dyna_headers_M(ActRecord);

typedef struct {
	ActRecordDyna act_history;
	int undo_mode;
	int action_complete;
	int bonus_action_complete;
	int free_action_complete;
	int reaction_complete;
} TurnState;

TurnState* turn_state_init(TurnState*);
int turn_state_clear(TurnState*);

typedef struct {
	ItemDefDyna item_db;
	AbilityDefDyna ability_db;
	PlayerCharacter player;
	int is_ranged;
	ItemDef* selected_weapon;
	abilityType* selected_ability;
	//spell
	//int spell_slot;
	ActDyna act_stack;
	DamageDyna damages;
	TurnState turn_state;
} Blob;

Blob* get_blob();

void blob_init();
void blob_clear();

#endif
