#include <glib.h>
#include "../includes/data_manip_interpreter.h"
#include "../includes/glib-facade.h"
#include "../includes/blob.h"

void* exec_enable_spellcasting(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	(get_blob()->player.verbal_limits)++;
	(get_blob()->player.somatic_limits)++;
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_enable_concentration(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	(get_blob()->player.concentration_limits)++;
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_disable_spellcasting(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	(get_blob()->player.verbal_limits)--;
	(get_blob()->player.somatic_limits)--;
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_disable_concentration(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	(get_blob()->player.concentration_limits)--;
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
	ArbitraryState a;
	arbitrary_state_init(&a);
	a.private_name = holder;
	a.sequence = set_string(a.sequence, args->pdata[1]);
	ArbitraryStateDyna_add(asd, a);
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
	parse_sequence(asd->array[i].sequence);
	ArbitraryStateDyna_remove(asd, &(asd->array[i]), arbitrary_state_clear);
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_has_arbitrary_state(GPtrArray* args, returnType* return_type) {
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
	gboolean* results = malloc(sizeof(gboolean));
	*results = FALSE;
	ActDyna* a = &(get_blob()->player.acts);
	for (int i = 0; i < a->size; i++) {
		if (!strcmp(state_name, a->array[i].private_name)) {
			free(state_name);
			*results = TRUE;
			break;
		}
	}
	return results;
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
	g_str_int_hash_table_insert(
			get_blob()->player.arbitrary_counter,
			key, val);
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_val_of_arbitrary_counter(GPtrArray* args, returnType* return_type) {
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
	int* result = FAILED_FUNC_EXEC;
	int* val = g_str_int_hash_table_lookup(
			get_blob()->player.arbitrary_counter, key);
	free(key);
	if (val) {
		result = malloc(sizeof(int));
		*result = *val;
	}
	return result;
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

void* exec_selected_ability(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	abilityType* results = NULL;
	abilityType* a = get_blob()->selected_ability;
	if (a) {
		results = malloc(sizeof(abilityType));
		*results = *a;
	}
	return results;
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
	if (!strcmp(func_name, "has_arbitrary_state")) {
		*return_type = RTRN_BOOL;
		return exec_has_arbitrary_state;
	}
	if (!strcmp(func_name, "set_arbitrary_counter")) {
		*return_type = RTRN_EMPTY;
		return exec_set_arbitrary_counter;
	}
	if (!strcmp(func_name, "val_of_arbitrary_counter")) {
		*return_type = RTRN_INT;
		return exec_val_of_arbitrary_counter;
	}
	if (!strcmp(func_name, "add_action")) {
		*return_type = RTRN_EMPTY;
		return exec_add_action;
	}
	if (!strcmp(func_name, "selected_ability")) {
		*return_type = RTRN_ABLT;
		return exec_add_action;
	}
	*return_type = RTRN_FAIL;
	return FAILED_FUNC_EXEC;
}
