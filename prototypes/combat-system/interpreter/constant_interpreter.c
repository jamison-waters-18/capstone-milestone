#include <glib.h>
#include "../includes/data_manip_interpreter.h"
#include "../includes/item.h"

void* exec_ability_str(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	abilityType* ablt = malloc(sizeof(abilityType));
	*ablt = STR;
	return ablt;
}

void* exec_ability_dex(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	abilityType* ablt = malloc(sizeof(abilityType));
	*ablt = DEX;
	return ablt;
}

void* exec_ability_con(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	abilityType* ablt = malloc(sizeof(abilityType));
	*ablt = CON;
	return ablt;
}

void* exec_ability_int(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	abilityType* ablt = malloc(sizeof(abilityType));
	*ablt = INT;
	return ablt;
}

void* exec_ability_wis(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	abilityType* ablt = malloc(sizeof(abilityType));
	*ablt = WIS;
	return ablt;
}

void* exec_ability_cha(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	abilityType* ablt = malloc(sizeof(abilityType));
	*ablt = CHA;
	return ablt;
}

void* exec_bonus_action(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	actType* a = malloc(sizeof(actType));
	*a = BONUS_ACTION;
	return a;
}

void* exec_menu_ability_check(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	menuType* m = malloc(sizeof(menuType));
	*m = ABILITY_CHECK;
	return m;
}

void* exec_after_turn(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	processLocationType* p = malloc(sizeof(processLocationType));
	*p = AFTER_TURN;
	return p;
}

void* exec_before_turn(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	processLocationType* p = malloc(sizeof(processLocationType));
	*p = BEFORE_TURN;
	return p;
}

void* exec_when_taking_damage(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	processLocationType* p = malloc(sizeof(processLocationType));
	*p = TAKING_DAMAGE;
	return p;
}

void* exec_pre_weapon_damage(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	processLocationType* p = malloc(sizeof(processLocationType));
	*p = PRE_WEAPON_DAMAGE;
	return p;
}

void* exec_bludgeoning(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	damageType* d = malloc(sizeof(damageType));
	*d = BLUDGEONING;
	return d;
}

void* exec_piercing(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	damageType* d = malloc(sizeof(damageType));
	*d = PIERCING;
	return d;
}

void* exec_slashing(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	damageType* d = malloc(sizeof(damageType));
	*d = SLASHING;
	return d;
}

void* exec_weapon_flag_melee(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	int* f = malloc(sizeof(int));
	*f = WEAPON_FLAG_MELEE;
	return f;
}

funcType find_const_func(char* func_name, returnType* return_type) {
	if (!strcmp(func_name, "ability_str")) {
		*return_type = RTRN_ABLT;
		return exec_ability_str;
	}
	if (!strcmp(func_name, "ability_dex")) {
		*return_type = RTRN_ABLT;
		return exec_ability_dex;
	}
	if (!strcmp(func_name, "ability_con")) {
		*return_type = RTRN_ABLT;
		return exec_ability_con;
	}
	if (!strcmp(func_name, "ability_int")) {
		*return_type = RTRN_ABLT;
		return exec_ability_int;
	}
	if (!strcmp(func_name, "ability_wis")) {
		*return_type = RTRN_ABLT;
		return exec_ability_wis;
	}
	if (!strcmp(func_name, "ability_cha")) {
		*return_type = RTRN_ABLT;
		return exec_ability_cha;
	}
	if (!strcmp(func_name, "bonus_action")) {
		*return_type = RTRN_PHASE;
		return exec_bonus_action;
	}
	if (!strcmp(func_name, "menu_ability_check")) {
		*return_type = RTRN_MENU;
		return exec_menu_ability_check;
	}
	if (!strcmp(func_name, "after_turn")) {
		*return_type = RTRN_PROCESS_TIMING;
		return exec_after_turn;
	}
	if (!strcmp(func_name, "before_turn")) {
		*return_type = RTRN_PROCESS_TIMING;
		return exec_before_turn;
	}
	if (!strcmp(func_name, "when_taking_damage")) {
		*return_type = RTRN_PROCESS_TIMING;
		return exec_when_taking_damage;
	}
	if (!strcmp(func_name, "pre_weapon_damage")) {
		*return_type = RTRN_PROCESS_TIMING;
		return exec_pre_weapon_damage;
	}
	if (!strcmp(func_name, "bludgeoning")) {
		*return_type = RTRN_DAMAGE;
		return exec_bludgeoning;
	}
	if (!strcmp(func_name, "piercing")) {
		*return_type = RTRN_DAMAGE;
		return exec_piercing;
	}
	if (!strcmp(func_name, "slashing")) {
		*return_type = RTRN_DAMAGE;
		return exec_slashing;
	}
	if (!strcmp(func_name, "weapon_flag_melee")) {
		*return_type = RTRN_FLAG;
		return exec_weapon_flag_melee;
	}
	*return_type = RTRN_FAIL;
	return FAILED_FUNC_EXEC;
}
