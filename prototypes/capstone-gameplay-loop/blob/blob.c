#include "../includes/blob.h"
#include "../includes/parser.h"
#include "../includes/DynaImp.h"

ActRecord* act_record_init(ActRecord* ar) {
	if (!ar) return NULL;
	ar->private_name = NULL;
	ar->public_name = NULL;
	ar->undo_sequence = NULL;
	ar->act_type = ACTION;
	return ar;
}

int act_record_clear(ActRecord* ar) {
	if (!ar) return -1;
	//private and public names should be duplicate pointers
	free(ar->undo_sequence);
	ar->private_name = NULL;
	ar->public_name = NULL;
	ar->undo_sequence = NULL;
	act_record_init(ar);
	return 0;
}

int act_record_add_undo_command(char* new_command) {
	ActRecordDyna* ard = &(get_blob()->turn_state.act_history);
	if (!ard->size) {
		return -1;
	}
	ActRecord* current_act = &(ard->array[ard->size - 1]);
	char* result = set_string(NULL, new_command);
	if (current_act->undo_sequence) {
		result = append_string(result, current_act->undo_sequence);
		free(current_act->undo_sequence);
	}
	current_act->undo_sequence = result;
	return 0;
}

int act_record_undo() {
	TurnState* ts = &(get_blob()->turn_state);
	if (!ts->act_history.size) return -1;
	ts->undo_mode = 1;
	ActRecord* current_act = &(ts->act_history.array[ts->act_history.size - 1]);
	parse_sequence(current_act->undo_sequence);
	switch (current_act->act_type) {
	case ACTION:
	case LONG:
		ts->action_complete -= 1;
		break;
	case BONUS_ACTION:
		ts->bonus_action_complete -= 1;
		break;
	case FREE_ACTION:
		ts->free_action_complete -= 1;
		break;
	case REACTION:
		ts->reaction_complete -= 1;
		break;
	default: break;
	}
	ActRecordDyna_remove(&(ts->act_history), current_act, act_record_clear);
	ts->undo_mode = 0;
	return 0;
}

generate_dyna_functions_M(ActRecord);

TurnState* turn_state_init(TurnState* ts) {
	if (!ts) return NULL;
	ts->undo_mode = 0;
	ts->turn_began = 0;
	ts->act_aborted = 0;
	ts->action_complete = 0;
	ts->bonus_action_complete = 0;
	ts->free_action_complete = 0;
	ts->reaction_complete = 0;
	ActRecordDyna_init(&(ts->act_history));
	return ts;
}

int turn_state_clear(TurnState* ts) {
	if (!ts) return -1;
	ts->undo_mode = 0;
	ts->turn_began = 0;
	ts->act_aborted = 0;
	ts->action_complete = 0;
	ts->bonus_action_complete = 0;
	ts->free_action_complete = 0;
	ts->reaction_complete = 0;
	ActRecordDyna_clear(&(ts->act_history), act_record_clear);
	return 0;
}

Blob* get_blob() {
	static Blob blob;
	return &blob;
}

void blob_init() {
	Blob* b = get_blob();
	ItemDefDyna_init(&(b->item_db));
	player_character_init(&(b->player));
	DamageDyna_init(&(b->damages));
	b->selected_ability = NULL;
	b->selected_weapon = NULL;
	b->is_ranged = 0;
	turn_state_init(&(b->turn_state));
}

void blob_clear() {
	Blob* b = get_blob();
	ItemDefDyna_clear(&(b->item_db), item_def_clear);
	player_character_clear(&(b->player));
	ActDyna_clear(&(b->act_stack), act_clear);
	DamageDyna_clear(&(b->damages), NULL);
	free(b->selected_ability);
	turn_state_clear(&(b->turn_state));
}
