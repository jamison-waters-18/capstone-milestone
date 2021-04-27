#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "../includes/blob.h"
#include "../includes/import.h"
#include "../includes/parser.h"

void display_characters() {
	printf("This is the list of available characters:\n");
	DIR* folder;
	struct dirent* ent;
	if ((folder = opendir("data/saves")) != NULL) {
		while ((ent = readdir(folder)) != NULL) {
			if ( strlen(ent->d_name) > 4 ) {
				char* tag = &(ent->d_name[strlen(ent->d_name) - 4]);
				if (string_eq(tag, ".xml")) {
					*tag = '\0';
					printf("    %s\n", ent->d_name);
				}
			}
		}
		closedir (folder);
	}
}

void turn() {
	TurnState* ts = &(get_blob()->turn_state);
	if (!ts->turn_began) {
		turn_state_init(ts);
		ts->turn_began = 1;
		execute_player_processes(&(get_blob()->player), BEFORE_TURN);
	}
	char* input = NULL;
	while(1) {
		if (ts->act_history.size) {
			printf("\nHere are the acts taken so far:\n");
			for (int i = 0; i < ts->act_history.size; i++) {
				printf("    %s\n", ts->act_history.array[i].public_name);
			}
		}
		printf("\nPlease select the type of action you would like to perform:\n");
		if(!ts->action_complete) {
			printf("1) Action\n");
		}
		if(!ts->bonus_action_complete) {
			printf("2) Bonus Action\n");
		}
		if(!ts->free_action_complete) {
			printf("3) Free Action\n");
		}
		printf("4) Undo\n");
		printf("5) Cancel\n");
		printf("6) Finish\n");
		printf("> ");

		input = read_file(stdin);
		if (!input) {
			printf("Error: Failed Mem Alloc\n");
			continue;
		}
		if (strlen(input) < 1) {
			free(input);
			printf("Error: Invalid Input\n");
			continue;
		}
		char selection = input[0];
		free(input);
		actType act_type;
		if (selection == '1' && !ts->action_complete) {
			act_type = ACTION;
		} else if (selection == '2' && !ts->bonus_action_complete) {
			act_type = BONUS_ACTION;
		} else if (selection == '3' && !ts->free_action_complete) {
			act_type = FREE_ACTION;
		} else if (selection == '4') {
			act_record_undo();
			continue;
		} else if (selection == '5') {
			while(ts->act_history.size) {
				act_record_undo();
			}
			return;
		} else if (selection == '6') {
			ActRecordDyna_clear(&(ts->act_history), act_record_clear);
			execute_player_processes(&(get_blob()->player), AFTER_TURN);
			turn_state_clear(ts);
			return;
		} else {
			printf("Error: Unexpected Symbol '%c'\n", selection);\
			continue;
		}
		ActDyna* acts = &(get_blob()->player.acts);

		char* inner_input = NULL;
		while (1) {
			printf("\nHere are the available options:\n");
			for(int i = 0; i < acts->size; i++) {
				if (acts->array[i].duration == act_type) {
					returnType rt;
					int* can_run = (int*) eval_expr(acts->array[i].prereqs, &rt);
					if (rt == RTRN_BOOL && *can_run) {
						printf("    %s\n", acts->array[i].public_name);
					}
					free(can_run);
				}
			}
			printf("    cancel\n");
			printf("> ");
			free(inner_input);
			inner_input = read_file(stdin);
			if (!inner_input) {
				printf("Mem Alloc Error\n");
				continue;
			}
			if (string_eq(inner_input, "cancel")) {
				break;
			}
			title_string(inner_input);
			int i = 0;
			for (i = 0; i < acts->size; i++) {
				if (string_eq(inner_input, acts->array[i].public_name)) {
					returnType rt;
					int* can_run_ptr = (int*) eval_expr(acts->array[i].prereqs, &rt);
					int can_run = *can_run_ptr;
					free(can_run_ptr);
					if (rt == RTRN_BOOL && can_run) {
						break;
					}
				}
			}
			if ( i < acts->size ) {
				ActRecord ar;
				act_record_init(&ar);
				ar.private_name = acts->array[i].private_name;
				ar.public_name = acts->array[i].public_name;
				ar.act_type = act_type;
				ActRecordDyna_add(&(ts->act_history), ar);
				parse_sequence(acts->array[i].sequence);

				if (act_type == ACTION) ts->action_complete += 1;
				else if (act_type == BONUS_ACTION) ts->bonus_action_complete += 1;
				else if (act_type == FREE_ACTION) ts->free_action_complete += 1;

				if (ts->act_aborted) {
					act_record_undo();
					ts->act_aborted = 0;
				}

				break;
			}
			else {
				printf("Unknow Act %s\n", inner_input);
			}
		}
		free(inner_input);
	}
}

void damage() {
	char* input = NULL;
	char damage_type[15];
	char leftovers[2];
	int value;
	damageType type;
	int input_valid = 0;
	
	while(!input_valid) {
		printf("What was the damage? (like \"15 fire\")\n");
		printf("> ");
		free(input);
		input = read_file(stdin);
		int num_bits = sscanf(input, "%d %s %s",
				&value, damage_type, leftovers);
		switch (num_bits) {
		case 0:
			printf("Invalid damage value\n");
			break;
		case 1:
			printf("No damage type provided\n");
			break;
		case 2:
			if (value < 1) {
				printf("Damage must be positive\n");
			}
			lower_string(damage_type);
			type = str_to_damage_type(damage_type);
			if (type != FORCE || string_eq(damage_type, "force")) {
				input_valid = 1;
				free(input);
			}
			break;
		default:
			printf("Improper number of arguments\n");
		}
	}
	int damage_index = (int) type;
	PlayerCharacter* p = &(get_blob()->player);
	if (p->immunities[damage_index]) return;
	if (p->resistances[damage_index]) value /= 2;
	else if (p->vulnerables[damage_index]) value *= 2;

	DamageDyna* damages = &(get_blob()->damages);
	Damage formatted_damage = { type, { { value, 0, 0, 0, 0, 0, 0, 0 } } };
	DamageDyna_add(damages, formatted_damage);

	execute_player_processes(p, TAKING_DAMAGE);

	p->hp -= damages->array[0].dice.die[0];

	DamageDyna_remove(damages, damages->array, NULL);

	if (p->hp <= 0) {
		if(p->hp + player_max_hp(p) < 1) {
			p->hp = -1;
			printf("\n%s has passed away. Further play requires resurrection\n", p->public_name);
		}
		else {
			p->hp = 0;
			printf("\n%s has fallen unconscious!\n", p->public_name);
			execute_player_processes(p, DOWNED);
		}
	}
}

void equip() {
	PlayerCharacter* p = &(get_blob()->player);
	//ItemDefDyna* idd = &(get_blob()->item_db);
	while (1) {
		printf("\nPlease select the desired slot (by number):\n");
		char empty_slot_name[] = "empty";
		char* armor_name = p->armor.public_name;
		char* main_hand_name = p->main_hand.public_name;
		char* off_hand_name = p->off_hand.public_name;

		if (!armor_name) armor_name = empty_slot_name;
		if (!main_hand_name) main_hand_name = empty_slot_name;
		if (!off_hand_name) off_hand_name = empty_slot_name;
		printf("1) Armor (%s)\n", armor_name);
		printf("2) Main Hand (%s)\n", main_hand_name);
		printf("3) Off Hand (%s)\n", off_hand_name);
		printf("4) Quit\n");
		printf("> ");
		char* input = read_file(stdin);
		if (!input) {
			printf("Mem Alloc Failure\n");
			continue;
		}
		char selection = input[0];
		free(input);

		ItemRef* selected_item = NULL;
		switch (selection) {
		case '1':
			selected_item = &(p->armor);
			break;
		case '2':
			selected_item = &(p->main_hand);
			break;
		case '3':
			selected_item = &(p->off_hand);
			break;
		case '4':
			return;
		default:
			printf("Error: Unexpected Symbol '%c'\n", selection);
		}

		if (!selected_item) continue;

		if (selected_item->private_name) {
			ItemRefDyna_add(&(p->inventory), *selected_item);
			item_ref_init(selected_item);
			printf("Item successfully stowed.\n");
			continue;
		}

		printf("These are your items.\n");
		int i;
		for(i = 0; i < p->inventory.size; i++) {
			printf("    %s\n", p->inventory.array[i].public_name);
		}
		while(1) {
			printf("What would you like to use? > ");
			input = title_string(strip_string(read_file(stdin)));

			if(string_eq(input, "quit")) {
				break;
			}

			for(i = 0; i < p->inventory.size; i++) {
				if(string_eq(input, p->inventory.array[i].public_name)) {
					*selected_item = p->inventory.array[i];
					ItemRefDyna_remove(&(p->inventory), &(p->inventory.array[i]), NULL);
					goto INV_ITEM_SELECTION_BREAK_OUT;
				}
			}
			printf("Unrecognized item name.\n");
			free(input);
		}
		INV_ITEM_SELECTION_BREAK_OUT: free(input);
	}
}

void long_rest() {
	PlayerCharacter* p = &(get_blob()->player);

	p->hp = player_max_hp(p);

	for (int i = 0; i < 10; i++) {
		p->spell_slots.slot[i] = p->spell_slots_max.slot[i];
	}

	//restore up to half of hit dice;
	int total_hit_dice = 0;
	int missing_hit_dice = 0;
	Dice max_dice = player_max_hit_dice(p);
	int dice_shortage[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 8; i++) {
		total_hit_dice += max_dice.die[i];
		dice_shortage[i] = max_dice.die[i] - p->hit_dice.die[i];
		missing_hit_dice += dice_shortage[i];
	}
	total_hit_dice /= 2;
	if (total_hit_dice == 0) total_hit_dice = 1;
	if (total_hit_dice < missing_hit_dice) missing_hit_dice = total_hit_dice;
	for (int i = 7; i >= 0 && missing_hit_dice > 0; i--) {
		if (!dice_shortage[i]) continue;
		if (dice_shortage[i] > missing_hit_dice) {
			p->hit_dice.die[i] += missing_hit_dice;
			break;
		}
		p->hit_dice.die[i] = max_dice.die[i];
		missing_hit_dice -= dice_shortage[i];
	}

	execute_player_processes(p, LONG_REST);
}

void game_loop() {
	char* input = NULL;
	PlayerCharacter* p = &(get_blob()->player);
	while (1) {
		printf("\nStats:\n");
		printf("hp: %d/%d\n", p->hp, player_max_hp(p));
		printf("ac: %d\n", player_ac(p));
		//TODO
		printf("proficiency bonus: %d\n", level_to_proficiency(xp_to_level(p->xp)));
		printf("STR: %d (%d)\n", p->ability_scores[0], ability_score_to_mod(p->ability_scores[0]));
		printf("DEX: %d (%d)\n", p->ability_scores[1], ability_score_to_mod(p->ability_scores[1]));
		printf("CON: %d (%d)\n", p->ability_scores[2], ability_score_to_mod(p->ability_scores[2]));
		printf("INT: %d (%d)\n", p->ability_scores[3], ability_score_to_mod(p->ability_scores[3]));
		printf("WIS: %d (%d)\n", p->ability_scores[4], ability_score_to_mod(p->ability_scores[4]));
		printf("CHA: %d (%d)\n", p->ability_scores[5], ability_score_to_mod(p->ability_scores[5]));
		ItemDef* ref;
		if (p->armor.private_name) {
			ref = NULL;
			for (int i = 0; i < get_blob()->item_db.size; i++) {
				if (string_eq(p->armor.private_name, get_blob()->item_db.array[i].private_name)) {
					ref = &(get_blob()->item_db.array[i]);
					break;
				}
			}
			if (!ref) {
				printf("Unknown armor %s\n", p->armor.private_name);
			}
			else {
				printf("armor: %s\n", ref->public_name);
			}
		}
		if (p->main_hand.private_name) {
			ref = NULL;
			for (int i = 0; i < get_blob()->item_db.size; i++) {
				if (string_eq(p->main_hand.private_name, get_blob()->item_db.array[i].private_name)) {
					ref = &(get_blob()->item_db.array[i]);
					break;
				}
			}
			if (!ref) {
				printf("Unknown main hand %s\n", p->armor.private_name);
			}
			else {
				printf("main hand: %s\n", ref->public_name);
			}
		}
		if (p->off_hand.private_name) {
			ref = NULL;
			for (int i = 0; i < get_blob()->item_db.size; i++) {
				if (string_eq(p->off_hand.private_name, get_blob()->item_db.array[i].private_name)) {
					ref = &(get_blob()->item_db.array[i]);
					break;
				}
			}
			if (!ref) {
				printf("Unknown off hand %s\n", p->armor.private_name);
			}
			else {
				printf("off hand: %s\n", ref->public_name);
			}
		}
		printf("States:\n");
		print_player_states(p);
		printf("Counters:\n");
		print_player_counters(p);
		printf("\nPlease select the desired option (by number):\n");
		printf("1) Perform Turn Of Combat\n");
		printf("2) Receive Damage\n");
		printf("3) Change Equipment Loadout\n");
		printf("4) Long Rest\n");
		printf("5) Check For New Abilities\n");
		printf("6) Quit\n");
		printf("> ");

		input = read_file(stdin);
		if (!input) {
			printf("Error: Failed Mem Alloc\n");
			continue;
		}
		if (strlen(input) < 1) {
			free(input);
			printf("Error: Invalid Input\n");
			continue;
		}
		char selection = input[0];
		free(input);

		switch (selection) {
		case '1':
			turn();
			break;
		case '2':
			damage();
			break;
		case '3':
			equip();
			break;
		case '4':
			long_rest();
			break;
		case '5':;
			AbilityDefDyna* abltdd = &(get_blob()->ability_db);
			for (int i = 0; i < p->dormant_abilities.size; i++) {
				for (int j = 0; j < abltdd->size; j++) {
					if (string_eq(p->dormant_abilities.array[i].private_name,
								abltdd->array[j].private_name)) {
						returnType rt;
						int* can_run = (int*) eval_expr(abltdd->array[j].prereqs, &rt);
						if (rt == RTRN_BOOL && *can_run) {
							printf("\nYou've learned %s\n", abltdd->array[j].public_name);
							parse_sequence(abltdd->array[j].sequence);
							AbilityRefDyna_add(&(p->active_abilities),
										p->dormant_abilities.array[i]);
							AbilityRefDyna_remove(&(p->dormant_abilities),
										&(p->dormant_abilities.array[i]), NULL);
						}
						free(can_run);
					}
				}
			}
			break;
		case '6':
			return;
		default:
			printf("Error: Unexpected Symbol '%c'\n", selection);
		}
	}
}

int main(int argc, char** argv) {
	Blob* b = get_blob();
	blob_init();
	import_items();
	import_abilities();
	char* name = NULL;
	while (1) {
		display_characters();
		printf("What is the character you would like to load? > ");
		free(name);
		name = read_file(stdin);
		if (name) {
			if (string_eq(name, "quit")) {
				free(name);
				return 0;
			}
			import_character(name);
			if (b->player.private_name) break;
			printf("Character \"%s\" not found.\n", name);
		}
	}
	free(name);
	printf("%s imported successfully.\n", b->player.public_name);
	
	game_loop();
	blob_clear();
	return 0;
}
