#include <glib.h>
#include "../includes/interpreter.h"
#include "../includes/glib-facade.h"
#include "../includes/blob.h"


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
	ArbitraryStateDyna* a = &(get_blob()->player.arbitrary_states);
	gboolean* results = malloc(sizeof(gboolean));
	*results = FALSE;
	for (int i = 0; i < a->size; i++) {
		if (!strcmp(state_name, a->array[i].private_name)) {
			*results = TRUE;
			break;
		}
	}
	free(state_name);
	return results;
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
	else {
		*return_type = RTRN_FAIL;
	}
	return result;
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
	} else {
		*return_type = RTRN_FAIL;
	}
	return results;
}

void* exec_weapon_damage_type(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	DamageDyna* d = &(get_blob()->damages);
	for (int i = 0; i < d->size; i++) {
		if	(	d->array[i].type == BLUDGEONING ||
				d->array[i].type == PIERCING ||
				d->array[i].type == SLASHING) {
			damageType* result = malloc(sizeof(damageType));
			*result = d->array[i].type;
			return result;
		}
	}
	*return_type = RTRN_FAIL;
	return FAILED_FUNC_EXEC;
}

void* exec_weapon_flags(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}

	int* flags = malloc(sizeof(int));
	if(!get_blob()->selected_weapon) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	*flags = get_blob()->selected_weapon->flags;
	return flags;
}

void* exec_attack_flags(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}

	int* flags = malloc(sizeof(int));
	*flags = get_blob()->wield_mode;
	return flags;
}

funcType find_data_lookup_func(char* func_name, returnType* return_type) {
	//data checking functions
	if (!strcmp(func_name, "has_arbitrary_state")) {
		*return_type = RTRN_BOOL;
		return exec_has_arbitrary_state;
	}
	if (!strcmp(func_name, "val_of_arbitrary_counter")) {
		*return_type = RTRN_INT;
		return exec_val_of_arbitrary_counter;
	}
	if (!strcmp(func_name, "selected_ability")) {
		*return_type = RTRN_ABLT;
		return exec_selected_ability;
	}
	if (!strcmp(func_name, "weapon_damage_type")) {
		*return_type = RTRN_DAMAGE;
		return exec_weapon_damage_type;
	}
	if (!strcmp(func_name, "weapon_flags")) {
		*return_type = RTRN_FLAG;
		return exec_weapon_flags;
	}
	if (!strcmp(func_name, "attack_flags")) {
		*return_type = RTRN_FLAG;
		return exec_attack_flags;
	}
	*return_type = RTRN_FAIL;
	return FAILED_FUNC_EXEC;
}
