#include <glib.h>
#include "../includes/interpreter.h"
#include "../includes/glib-facade.h"
#include "../includes/blob.h"

void* exec_enable_spellcasting(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	(get_blob()->player.verbal_limits)++;
	(get_blob()->player.somatic_limits)++;
	if (!get_blob()->turn_state.undo_mode) {
		act_record_add_undo_command("disable_spellcasting();");
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_enable_concentration(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	(get_blob()->player.concentration_limits)++;
	if (!get_blob()->turn_state.undo_mode) {
		act_record_add_undo_command("disable_concentration();");
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_disable_spellcasting(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	(get_blob()->player.verbal_limits)--;
	(get_blob()->player.somatic_limits)--;
	if (!get_blob()->turn_state.undo_mode) {
		act_record_add_undo_command("enable_spellcasting();");
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_disable_concentration(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	(get_blob()->player.concentration_limits)--;
	if (!get_blob()->turn_state.undo_mode) {
		act_record_add_undo_command("enable_concentration();");
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_add_note(GPtrArray* args, returnType* return_type) {
	if (args->len != 4) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	returnType sub_return_type;
	menuType* m = eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_MENU) {
		safe_free(m);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	
	Note n;
	note_init(&n);
	n.menu = *m;
	free(m);
	n.private_name = eval_expr(args->pdata[1], &sub_return_type);
	if (sub_return_type != RTRN_STR) {
		safe_free(n.private_name);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	n.message = eval_expr(args->pdata[3], &sub_return_type);
	if (sub_return_type != RTRN_STR) {
		free(n.private_name);
		safe_free(n.message);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	n.prereqs = set_string(n.prereqs, args->pdata[2]);
	NoteDyna_add(&(get_blob()->player.notes), n);
	if (!get_blob()->turn_state.undo_mode) {
		char* command = set_string_f(NULL, "remove_note(`%s`);", n.private_name); 
		act_record_add_undo_command(command);
		free(command);
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_remove_note(GPtrArray* args, returnType* return_type) {
	if (args->len != 1) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	returnType sub_return_type;
	char* note_name = eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_STR) {
		safe_free(note_name);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	NoteDyna* nd = &(get_blob()->player.notes);
	int i = 0;
	for (; i < nd->size; i++) {
		if (!strcmp(note_name, nd->array[i].private_name)) {
			if (!get_blob()->turn_state.undo_mode) {
				char menu_name[50];
				switch (nd->array[i].menu) {
				case MAIN_SCREEN:
					strcpy(menu_name, "menu_main_screen()"); break;
				case ABILITY_SAVE:
					strcpy(menu_name, "menu_ability_save()"); break;
				case ABILITY_CHECK:
					strcpy(menu_name, "menu_ability_check()"); break;
				case SKILL_CHECK:
					strcpy(menu_name, "menu_skill_check()"); break;
				case INITIATIVE:
					strcpy(menu_name, "menu_initiative()"); break;
				case APPLYING_STATUS_EFFECT:
					strcpy(menu_name, "menu_status_effect()"); break;
				case COMBAT_MENU:
					strcpy(menu_name, "menu_combat()"); break;
				}
				char* command = set_string_f(NULL,
						"add_note(%s, `%s`, %s, `%s`);", menu_name, note_name, nd->array[i].prereqs, nd->array[i].message); 
				act_record_add_undo_command(command);
				free(command);
			}
			NoteDyna_remove(nd, &(nd->array[i]), note_clear);
			i = nd->size + 1;
			break;
		}
	}
	free(note_name);
	if (i == nd->size) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_add_process(GPtrArray* args, returnType* return_type) {
	if (args->len != 5) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	returnType sub_return_type;
	processLocationType* plt = eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_PROCESS_TIMING) {
		safe_free(plt);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	
	Process p;
	process_init(&p);
	p.when_to_run = *plt;
	free(plt);
	p.private_name = eval_expr(args->pdata[1], &sub_return_type);
	if (sub_return_type != RTRN_STR) {
		safe_free(p.private_name);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	p.description = eval_expr(args->pdata[2], &sub_return_type);
	if (sub_return_type != RTRN_STR) {
		free(p.private_name);
		safe_free(p.description);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	p.prereqs = set_string(p.prereqs, args->pdata[3]);
	p.sequence = set_string(p.sequence, args->pdata[4]);
	ProcessDyna_add(&(get_blob()->player.processes), p);
	if (!get_blob()->turn_state.undo_mode) {
		char* command = set_string_f(NULL, "remove_process(`%s`);", p.private_name); 
		act_record_add_undo_command(command);
		free(command);
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_remove_process(GPtrArray* args, returnType* return_type) {
	if (args->len != 1) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	returnType sub_return_type;
	char* process_name = eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_STR) {
		safe_free(process_name);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	ProcessDyna* pd = &(get_blob()->player.processes);
	int i = 0;
	for (; i < pd->size; i++) {
		if (!strcmp(process_name, pd->array[i].private_name)) {
			if (!get_blob()->turn_state.undo_mode) {
				char location_name[50];
				switch (pd->array[i].when_to_run) {
				case SHORT_REST:
					strcpy(location_name, "short_rest()"); break;
				case LONG_REST:
					strcpy(location_name, "long_rest()"); break;
				case BEFORE_TURN:
					strcpy(location_name, "before_turn()"); break;
				case AFTER_TURN:
					strcpy(location_name, "after_turn()"); break;
				case AFTER_TAKING_STATUS_EFFECT:
					strcpy(location_name, "after_taking_status_effect()"); break;
				case BEFORE_COMBAT:
					strcpy(location_name, "before_combat()"); break;
				case DOWNED:
					strcpy(location_name, "downed()"); break;
				case TAKING_DAMAGE:
					strcpy(location_name, "taking_damage()"); break;
				case PRE_WEAPON_ATTACK:
					strcpy(location_name, "pre_weapon_attack()"); break;
				case WEAPON_ATTACK_OPTIONS:
					strcpy(location_name, "weapon_attack_options()"); break;
				case PRE_WEAPON_DAMAGE:
					strcpy(location_name, "pre_weapon_damage()"); break;
				case WEAPON_DAMAGE_OPTIONS:
					strcpy(location_name, "weapon_damage_options()"); break;
				case PRE_SPELLCAST:
					strcpy(location_name, "pre_spellcast()"); break;
				case SPELLCAST_OPTIONS:
					strcpy(location_name, "spellcast_options()"); break;
				case PRE_SPELL_DAMAGE:
					strcpy(location_name, "pre_spell_damage()"); break;
				case SPELL_DAMAGE_OPTIONS:
					strcpy(location_name, "spell_damage_options()"); break;
				}
				char* command = set_string_f(NULL,
						"add_note(%s, `%s`, `%s`, %s, %s);", location_name, process_name, pd->array[i].description, pd->array[i].prereqs, pd->array[i].sequence); 
				act_record_add_undo_command(command);
				free(command);
			}
			ProcessDyna_remove(pd, &(pd->array[i]), process_clear);
			i = pd->size + 1;
			break;
		}
	}
	free(process_name);
	if (i == pd->size) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_enter_arbitrary_state(GPtrArray* args, returnType* return_type) {
	if (args->len != 2) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	char* holder = NULL;
	returnType sub_return_type;
	holder = eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_STR) {
		safe_free(holder);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}

	ArbitraryStateDyna* asd = &(get_blob()->player.arbitrary_states);
	for (int i = 0; i < asd->size; i++) {
		if (!strcmp(holder, asd->array[i].private_name)) {
			free(holder);
			return SUCCESSFUL_EMPTY_FUNC_EXEC;
		}
	}
	ArbitraryState a;
	arbitrary_state_init(&a);
	a.private_name = holder;
	a.sequence = set_string(a.sequence, args->pdata[1]);
	ArbitraryStateDyna_add(asd, a);
	if (!get_blob()->turn_state.undo_mode) {
		char* command = set_string_f(NULL, "exit_arbitrary_state(`%s`);", a.private_name); 
		act_record_add_undo_command(command);
		free(command);
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_exit_arbitrary_state(GPtrArray* args, returnType* return_type) {
	if (args->len != 1) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	returnType sub_return_type;
	char* state_name = eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_STR) {
		safe_free(state_name);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	ArbitraryStateDyna* asd = &(get_blob()->player.arbitrary_states);
	int i = 0;
	for (; i < asd->size; i++) {
		if (!strcmp(state_name, asd->array[i].private_name)) {
			break;
		}
	}
	free(state_name);
	if (i == asd->size) {
		return SUCCESSFUL_EMPTY_FUNC_EXEC;
	}

	if (!get_blob()->turn_state.undo_mode) {
		parse_sequence(asd->array[i].sequence);
		char* command = set_string_f(NULL,
				"enter_arbitrary_state(`%s`, %s);", asd->array[i].private_name, asd->array[i].sequence); 
		act_record_add_undo_command(command);
		free(command);
	}

	ArbitraryStateDyna_remove(asd, &(asd->array[i]), arbitrary_state_clear);
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_set_arbitrary_counter(GPtrArray* args, returnType* return_type) {
	if (args->len != 2) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	returnType sub_return_type;
	char* key = eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_STR) {
		safe_free(key);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	int* val = eval_expr(args->pdata[1], &sub_return_type);
	if (sub_return_type != RTRN_INT) {
		free(key);
		safe_free(val);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	
	if (!get_blob()->turn_state.undo_mode) {
		int* prior_val = g_str_int_hash_table_lookup(
				get_blob()->player.arbitrary_counter, key);
		char* command = NULL;
		if (prior_val) {
			command = set_string_f(NULL,
					"set_arbitrary_counter(%s, %d);", args->pdata[0], *prior_val); 
		} else {
			command = set_string_f(NULL,
					"remove_arbitrary_counter(%s);", args->pdata[0]); 
		}
		act_record_add_undo_command(command);
		free(command);
	}
	g_str_int_hash_table_insert(
			get_blob()->player.arbitrary_counter,
			key, val);
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_remove_arbitrary_counter(GPtrArray* args, returnType* return_type) {
	if (args->len != 1) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	returnType sub_return_type;
	char* key = eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_STR) {
		safe_free(key);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}

	int* prior_val = g_str_int_hash_table_lookup(
			get_blob()->player.arbitrary_counter, key);
	if (!prior_val) {
		free(key);
		return SUCCESSFUL_EMPTY_FUNC_EXEC;
	}

	if (!get_blob()->turn_state.undo_mode) {
		char* command = set_string_f(NULL,
				"set_arbitrary_counter(%s, %d);", args->pdata[0], *prior_val);
		act_record_add_undo_command(command);
		free(command);
	}
	g_str_int_hash_table_remove(
			get_blob()->player.arbitrary_counter,
			key);
	free(key);
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_add_action(GPtrArray* args, returnType* return_type) {
	if (args->len != 5) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	returnType sub_return_type;
	actType* holder = (actType*) eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_PHASE) {
		safe_free(holder);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	actType ablt = *holder;
	free(holder);
	
	char* holders[2] = {NULL, NULL};
	for (int i = 1; i < 3; i++) {
		returnType sub_return_type;
		holders[i-1] = eval_expr(args->pdata[i], &sub_return_type);
		if (sub_return_type != RTRN_STR) {
			safe_free(holders[0]);
			safe_free(holders[1]);
			*return_type = RTRN_FAIL;
			return FAILED_FUNC_EXEC;
		}
	}
	Act a;
	act_init(&a);
	a.duration = ablt;
	a.private_name = holders[0];
	a.public_name = holders[1];
	a.prereqs = set_string(a.prereqs, args->pdata[3]);
	a.sequence = set_string(a.sequence, args->pdata[4]);
	Blob* b = get_blob();
	ActDyna_add(&(b->player.acts), a);
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_add_to_damage(GPtrArray* args, returnType* return_type) {
	if (args->len != 3) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	returnType sub_return_type;
	void* holder = eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_DAMAGE) {
		safe_free(holder);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	damageType dmg_type = *((damageType*) holder);
	free(holder);
	holder = eval_expr(args->pdata[1], &sub_return_type);
	if (sub_return_type != RTRN_INT) {
		safe_free(holder);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	int dmg_val = *((int*) holder);
	free(holder);
	holder = eval_expr(args->pdata[2], &sub_return_type);
	if (sub_return_type != RTRN_INT) {
		safe_free(holder);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	int dmg_die = *((int*) holder);
	free(holder);
	dmg_die = dicenum_to_index(dmg_die);
	if (dmg_die == -1) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	DamageDyna* d = &(get_blob()->damages);
	for (int i = 0; i < d->size; i++) {
		if (d->array[i].type == dmg_type) {
			d->array[i].dice.die[dmg_die] += dmg_val;
			return SUCCESSFUL_EMPTY_FUNC_EXEC;
		}
	}
	Damage new_dmg;
	damage_init(&new_dmg);
	new_dmg.type = dmg_type;
	new_dmg.dice.die[dmg_die] = dmg_val;
	DamageDyna_add(d, new_dmg);
	if (!get_blob()->turn_state.undo_mode) {
		char* command = set_string_f(NULL,
				"add_to_damage(%s, %s, %d);", args->pdata[0], args->pdata[1], dmg_val * -1); 
		act_record_add_undo_command(command);
		free(command);
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_add_resistance(GPtrArray* args, returnType* return_type) {
	if (!args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}

	int* resistances = &(get_blob()->player.resistances[0]);
	for (int i = 0; i < args->len; i++) {
		returnType sub_return_type;
		void* holder = eval_expr(args->pdata[i], &sub_return_type);
		if (sub_return_type != RTRN_DAMAGE) {
			safe_free(holder);
			*return_type = RTRN_FAIL;
			return FAILED_FUNC_EXEC;
		}
		damageType dmg_type = *((damageType*) holder);
		free(holder);

		resistances[(int) dmg_type] += 1;
	}

	if (!get_blob()->turn_state.undo_mode) {
		char* command = set_string(NULL, "remove_resistance(");
		command = append_string(command, args->pdata[0]);
		for (int i = 1; i < args->len; i++) {
			command = append_string(command, ", ");
			command = append_string(command, args->pdata[i]);
		}
		command = append_string(command, ");");
		act_record_add_undo_command(command);
		free(command);
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_remove_resistance(GPtrArray* args, returnType* return_type) {
	if (!args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}

	int* resistances = &(get_blob()->player.resistances[0]);
	for (int i = 0; i < args->len; i++) {
		returnType sub_return_type;
		void* holder = eval_expr(args->pdata[0], &sub_return_type);
		if (sub_return_type != RTRN_DAMAGE) {
			safe_free(holder);
			*return_type = RTRN_FAIL;
			return FAILED_FUNC_EXEC;
		}
		damageType dmg_type = *((damageType*) holder);
		free(holder);

		resistances[(int) dmg_type] += 1;
	}

	if (!get_blob()->turn_state.undo_mode) {
		char* command = set_string(NULL, "add_resistance(");
		command = append_string(command, args->pdata[0]);
		for (int i = 1; i < args->len; i++) {
			command = append_string(command, ", ");
			command = append_string(command, args->pdata[i]);
		}
		command = append_string(command, ");");
		act_record_add_undo_command(command);
		free(command);
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

funcType find_data_manip_func(char* func_name, returnType* return_type) {
	//simple data manipulation functions
	if (!strcmp(func_name, "enable_spellcasting")) {
		*return_type = RTRN_EMPTY;
		return exec_enable_spellcasting;
	}
	if (!strcmp(func_name, "enable_concentration")) {
		*return_type = RTRN_EMPTY;
		return exec_enable_concentration;
	}
	if (!strcmp(func_name, "disable_spellcasting")) {
		*return_type = RTRN_EMPTY;
		return exec_disable_spellcasting;
	}
	if (!strcmp(func_name, "disable_concentration")) {
		*return_type = RTRN_EMPTY;
		return exec_disable_concentration;
	}
	//complex data manipulation functions
	if (!strcmp(func_name, "add_note")) {
		*return_type = RTRN_EMPTY;
		return exec_add_note;
	}
	if (!strcmp(func_name, "remove_note")) {
		*return_type = RTRN_EMPTY;
		return exec_remove_note;
	}
	if (!strcmp(func_name, "add_process")) {
		*return_type = RTRN_EMPTY;
		return exec_add_process;
	}
	if (!strcmp(func_name, "remove_process")) {
		*return_type = RTRN_EMPTY;
		return exec_remove_process;
	}
	if (!strcmp(func_name, "enter_arbitrary_state")) {
		*return_type = RTRN_EMPTY;
		return exec_enter_arbitrary_state;
	}
	if (!strcmp(func_name, "exit_arbitrary_state")) {
		*return_type = RTRN_EMPTY;
		return exec_exit_arbitrary_state;
	}
	if (!strcmp(func_name, "set_arbitrary_counter")) {
		*return_type = RTRN_EMPTY;
		return exec_set_arbitrary_counter;
	}
	if (!strcmp(func_name, "remove_arbitrary_counter")) {
		*return_type = RTRN_EMPTY;
		return exec_remove_arbitrary_counter;
	}
	if (!strcmp(func_name, "add_action")) {
		*return_type = RTRN_EMPTY;
		return exec_add_action;
	}
	if (!strcmp(func_name, "add_to_damage")) {
		*return_type = RTRN_EMPTY;
		return exec_add_to_damage;
	}
	if (!strcmp(func_name, "add_resistance")) {
		*return_type = RTRN_EMPTY;
		return exec_add_resistance;
	}
	if (!strcmp(func_name, "remove_resistance")) {
		*return_type = RTRN_EMPTY;
		return exec_remove_resistance;
	}
	*return_type = RTRN_FAIL;
	return FAILED_FUNC_EXEC;
}
