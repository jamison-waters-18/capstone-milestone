#include <glib.h>
#include "../includes/interpreter.h"
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

	ItemRef* selected_hand_ref = &(p->main_hand);
	ItemRef* other_hand_ref = &(p->off_hand);

	if ((b->wield_mode & WEAPON_FLAG_LIGHT)) {
		selected_hand_ref = &(p->off_hand);
		other_hand_ref = &(p->main_hand);
	}

	if (selected_hand_ref->public_name) {
		weapon_name = selected_hand_ref->private_name;
	}
	else {
		weapon_name = unarmed;
	}

	int i = 0;
	for(; i < b->item_db.size; i++) {
		if (!strcmp(weapon_name, b->item_db.array[i].private_name)) {
			b->selected_weapon = &(b->item_db.array[i]);
			break;
		}
	}

	if(i == b->item_db.size) {
		printf("Unknown weapon name %s\n", weapon_name);
		b->wield_mode = 0;
		b->turn_state.act_aborted = 1;
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}

	int is_weapon = 1;
	if (b->selected_weapon->category != ITEM_CATEGORY_WEAPON) {
		is_weapon = 0;
	}

	if (is_weapon && (b->selected_weapon->flags & WEAPON_FLAG_TWO_HANDED) && other_hand_ref->private_name) {
		b->selected_weapon = NULL;
		printf("%s requires a free hand to operate, but you're holding %s.\n ",
				selected_hand_ref->public_name, other_hand_ref->public_name);
		b->wield_mode = 0;
		b->turn_state.act_aborted = 1;
		return SUCCESSFUL_EMPTY_FUNC_EXEC;
	}

	if (!selected_hand_ref->private_name) {
		b->wield_mode |= WEAPON_FLAG_MELEE;
	} else while (1) {
		printf("\nSwing, throw, or fire your %s? > ", b->selected_weapon->public_name);
		char* input = read_file(stdin);
		lower_string(input);
		if (!strcmp(input, "quit")) {
			b->selected_weapon = NULL;
			b->turn_state.act_aborted = 1;
			free(input);
			break;
		}
		if (!strcmp(input, "swing")) {
			free(input);
			b->wield_mode |= WEAPON_FLAG_MELEE;
			break;
		}
		if (!strcmp(input, "throw")) {
			free(input);
			b->wield_mode |= WEAPON_FLAG_THROWN;
			break;
		}
		if (!strcmp(input, "fire")) {
			free(input);
			if (!is_weapon || !(b->selected_weapon->flags & WEAPON_FLAG_AMMUNITION)) {
				printf("%s doesn't use ammo.\n", b->selected_weapon->public_name);
				continue;
			} else if (other_hand_ref->private_name) {
				b->selected_weapon = NULL;
				b->turn_state.act_aborted = 1;
				printf("%s requires a free hand to operate, but you're holding %s.\n ",
						selected_hand_ref->public_name, other_hand_ref->public_name);
				b->wield_mode = 0;
				b->turn_state.act_aborted = 1;
				return SUCCESSFUL_EMPTY_FUNC_EXEC;
			} else if (1) {
				//TODO: check for annumition
				printf("You don't have ammo for a %s.\n", b->selected_weapon->public_name);
				continue;
			} else {
				b->wield_mode |= WEAPON_FLAG_AMMUNITION;
				break;
			}
		}
		printf("Unrecognized input\n");
		free(input);
	}

	b->selected_ability = malloc(sizeof(abilityType));

	if (is_weapon && (b->wield_mode & WEAPON_FLAG_AMMUNITION)) {
		*(b->selected_ability) = DEX;
	} else if (!is_weapon || !(b->selected_weapon->flags & WEAPON_FLAG_FINESSE)) {
		*(b->selected_ability) = STR;
	} else {
		char* input;
		while (1) {
			printf("Use strength or dexterity? > ");
			input = read_file(stdin);
			lower_string(input);
			if (!strcmp(input, "quit")) {
				b->selected_weapon = NULL;
				free(b->selected_ability);
				b->selected_ability = NULL;
				b->wield_mode = 0;
				b->turn_state.act_aborted = 1;
				free(input);
				return FAILED_FUNC_EXEC;
			}
			if (strstr(input, "str")) {
				*(b->selected_ability) = STR;
				break;
			}
			if (strstr(input, "dex")) {
				*(b->selected_ability) = DEX;
				break;
			}
			printf("Unrecognized input\n");
			free(input);
		}
		free(input);
	}

	if (!b->selected_ability) {
		printf("Mem Alloc Fail\n");
		b->selected_weapon = NULL;
		free(b->selected_ability);
		b->selected_ability = NULL;
		b->wield_mode = 0;
		b->turn_state.act_aborted = 1;
		return FAILED_FUNC_EXEC;
	}

	int ability_mod = ability_score_to_mod(p->ability_scores[(int) *(b->selected_ability)]);
	Dice attack_dice = {{ ability_mod, 0, 0, 0, 0, 0, 1, 0 }};

	if (is_weapon) {
		for (i = 0; i < p->item_proficiencies->len; i++) {
			if (strstr(p->item_proficiencies->pdata[i], weapon_name)) {
				attack_dice.die[0] += level_to_proficiency(xp_to_level(p->xp));
				break;
			}
		}

		if (i == p->item_proficiencies->len) {
			for (i = 0; i < p->item_category_proficiencies.size; i++) {
				if (p->item_category_proficiencies.array[i].category == b->selected_weapon->category &&
						(p->item_category_proficiencies.array[i].flags & b->selected_weapon->flags)
						== p->item_category_proficiencies.array[i].flags) {
					attack_dice.die[0] += level_to_proficiency(xp_to_level(p->xp));
					break;
				}
			}
		}
	}

	execute_player_processes(&(b->player), PRE_WEAPON_ATTACK);

	/* TODO: check if you're proficient in the armor you're wearing. I'm not doing this until i shift to tables
	int is_armor_proficient = 0;
	if (p->armor.private_name) {
		for (i = 0; i < p->item_proficiencies->len; i++) {
			if (strstr(p->item_proficiencies->pdata[i], p->armor.private_name)) {
				is_armor_proficient = 1;
				break;
			}
		}

		if (!is_armor_proficient) {
			for (i = 0; i < p->item_category_proficiencies.size; i++) {
				if (p->item_category_proficiencies.array[i].category == b->selected_weapon->category &&
						(p->item_category_proficiencies.array[i].flags & b->selected_weapon->flags)
						== p->item_category_proficiencies.array[i].flags) {
					attack_dice.die[0] += level_to_proficiency(xp_to_level(p->xp));
					break;
				}
			}
		}
	} */

	int hit = 0;
	int crit = 0;
	char* dice_text = dice_to_string(&attack_dice);
	printf("\nRoll %s to attack\n", dice_text);
	while (1) {
		printf("Critical fail, miss, hit, or critical hit? > ");
		char* input = read_file(stdin);
		lower_string(input);
		if (!strcmp(input, "quit")) {
			b->selected_weapon = NULL;
			free(b->selected_ability);
			b->selected_ability = NULL;
			b->turn_state.act_aborted = 1;
			b->wield_mode = 0;
			free(dice_text);
			free(input);
			return SUCCESSFUL_EMPTY_FUNC_EXEC;
		}
		if (strstr(input, "crit")) {
			if (!strstr(input, "fail")) {
				hit = 1;
				crit = 1;
			}
			free(input);
			break;
		} else if (strstr(input, "miss")) {
			free(input);
			break;
		} else if (strstr(input, "hit")) {
			hit = 1;
			free(input);
			break;
		}
		free(input);
	}

	free(dice_text);
	dice_text = NULL;

	if (!hit) {
		b->selected_weapon = NULL;
		free(b->selected_ability);
		b->selected_ability = NULL;
		b->wield_mode = 0;
		return SUCCESSFUL_EMPTY_FUNC_EXEC;
	}

	if (!is_weapon || !((WEAPON_FLAG_MELEE | WEAPON_FLAG_THROWN | WEAPON_FLAG_AMMUNITION) & 
						b->selected_weapon->flags & b->wield_mode)) {
		Damage dmg = { BLUDGEONING, { { 0, 1, 0, 0, 0, 0, 0, 0 } } }; //TODO: In future, ask for damage type
		DamageDyna_add(&(b->damages), dmg);
	} else {
		DamageDyna* official_damage;
		if ((b->selected_weapon->flags & WEAPON_FLAG_VERSATILE) && !other_hand_ref->private_name) {
			official_damage = &(b->selected_weapon->sub.w->versatile_damage);
		} else {
			official_damage = &(b->selected_weapon->sub.w->damage);
		}
		for(int i = 0; i < official_damage->size; i++) {
			DamageDyna_add(&(b->damages), official_damage->array[i]);
		}
	}

	if(crit) {
		for (i = 0; i < b->damages.size; i++) {
			for (int j = 1; j < 8; j++) {
				b->damages.array[i].dice.die[j] *= 2;
			}
		}
	}

	if (!(b->wield_mode & WEAPON_FLAG_LIGHT) || ability_mod < 0) {
		b->damages.array[0].dice.die[0] += ability_mod;
	}

	execute_player_processes(&(b->player), PRE_WEAPON_DAMAGE);

	dice_text = damage_dyna_to_string(&(b->damages));
	
	printf("\nYou've dealt %s damage\n", dice_text);

	free(dice_text);
	b->selected_weapon = NULL;
	free(b->selected_ability);
	b->selected_ability = NULL;
	DamageDyna_clear(&(b->damages), NULL);

	//TODO: if an object is thrown, decrement player's posession of them, move to "on ground" list
	//will need to be done in-language, so this can be undone
	//TODO: if weapon fired, decrement ammunition
	//you can only pick up half of your fired ammunition, so, figure that out, i guess
	//	make a save file variable that alternates between 0 and 1, toggle it whenever ammunition is dropped, determine whther to store it

	b->wield_mode = 0;

	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

funcType find_ui_func(char* func_name, returnType* return_type) {
	if (!strcmp(func_name, "run_weapon_attack")) {
		*return_type = RTRN_EMPTY;
		return exec_run_weapon_attack;
	}
	*return_type = RTRN_FAIL;
	return FAILED_FUNC_EXEC;
}
