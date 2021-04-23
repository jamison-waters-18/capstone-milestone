#include <glib.h>
#include "../includes/ui_interpreter.h"
#include "../includes/blob.h"
#include "../includes/item.h"

void* exec_run_weapon_attack(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	Blob* b = get_blob();
	PlayerCharacter* p = &(b->player);

	char unarmed[] = "unarmed"; //TODO: make part of save file in future
	char* weapon_name;

	if (p->main_hand.private_name) {
		weapon_name = p->main_hand.private_name;
	}
	else {
		weapon_name = &(unarmed[0]);
	}

	int i = 0;
	for(; i < b->ability_db.size; i++) {
		if (!strcmp(weapon_name, b->ability_db.array[i].private_name)) {
			b->selected_weapon = &(b->ability_db.array[i]);
			break;
		}
	}

	if(i == b->ability_db.size) {
		printf("Unknown weapon name %s\n", weapon_name);
		b->turn_state.act_aborted = 1;
	}

	int ranged = 0;
	int two_handed = 0;
	while (p->main_hand.private_name) {
		printf("Melee or ranged attack for %s? > ", );
		char* input = read_file(stdin);
		lower_string(input);
		if (!strcmp(input, "quit")) {
			b->selected_weapon = NULL;
			b->turn_state.act_aborted = 1;
			free(input);
			break;
		}
		if (!strcmp(input, "melee")) {
			free(input);
			break;
		}
		if (!strcmp(input, "ranged")) {
			free(input);
			ranged = 1;
			break;
		}
		printf("Unrecognized input\n");
		free(input);
	}

	if (ranged && (b->selected_weapon->flags & WEAPON_FLAG_AMMUNITION)) {
		b->selected_ability = malloc(sizeof(abilityType));
		*(b->selected_ability) = DEX;
	} else if (!(b->selected_weapon->flags & WEAPON_FLAG_FINESSE)) {
		b->selected_ability = malloc(sizeof(abilityType));
		*(b->selected_ability) = STR;
	} else {
		while (1) {
			printf("Use strength or dexterity? > ", );
			char* input = read_file(stdin);
			lower_string(input);
			if (!strcmp(input, "quit")) {
				b->selected_weapon = NULL;
				free(b->selected_ability);
				b->selected_ability = NULL;
				b->turn_state.act_aborted = 1;
				free(input);
				break;
			}
			if (strstr(input, "str")) {
				free(input);
				b->selected_ability = malloc(sizeof(abilityType));
				*(b->selected_ability) = STR;
				break;
			}
			if (strstr(input, "dex")) {
				free(input);
				b->selected_ability = malloc(sizeof(abilityType));
				*(b->selected_ability) = DEX;
				break;
			}
			printf("Unrecognized input\n");
			free(input);
		}
	}

	//set up dice to roll (1d20 + ability_mod)
	//if name is in item-specific proficiencies
	//	add proficiency
	//else if item category and flags match any entry in item category proficiencies
	//	add proficiency

	execute_player_processes(&(b->player), PRE_WEAPON_ATTACK);

	//print out the dice to roll
	//prompt whether it was a crit fail, miss, hit, or crit

	//copy damage info from def
	//	if versatile and other hand empty, use that!

	//print out damage

	execute_player_processes(&(b->player), PRE_WEAPON_DAMAGE);

	b->selected_weapon = NULL;
	free(b->selected_ability);
	b->selected_ability = NULL;

	//TODO: if an object is thrown, decrement player's posession of them, move to "on ground" list
	//will need to be done in-language, so this can be undone

	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

funcType find_ui_func(char* func_name, returnType* return_type) {
	if (!strcmp(func_name, "run_weapon_attack")) {
		*return_type = RTRN_EMPTY;
		return exec_add;
	}
	*return_type = RTRN_FAIL;
	return FAILED_FUNC_EXEC;
}
