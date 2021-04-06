#ifndef BLOB_H
#define BLOB_H

#include "item.h"
#include "character.h"
#include "ability.h"

typedef struct {
	ItemDefDyna item_db;
	AbilityDefDyna ability_db;
	PlayerCharacter player;
	int is_ranged;
	//int proper_turn;
	ItemRef* selected_weapon;
	abilityType* selected_ability;
	//spell
	//int spell_slot;
	ActDyna act_stack;
	DamageDyna damages;
} Blob;

Blob* get_blob();

void blob_init();
void blob_clear();

#endif
