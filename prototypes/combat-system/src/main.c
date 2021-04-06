#include <stdio.h>
#include "../includes/item.h"
#include "../includes/blob.h"
#include "../includes/import.h"
#include "../includes/parser.h"

int main(int argc, char** argv) {
	blob_init();
	Blob* b = get_blob();
	import_items();
	import_abilities();
	import_character("shalta");
	/*things to do:
		apply effects of rage (using it)
		apply effects of ending it (using the other bonus action)
*/
	AbilityDef* rage_ability = NULL;
	for(int i = 0; i < b->ability_db.size; i++) {
		if (!strcmp("rage", b->ability_db.array[i].private_name)) {
			rage_ability = &(b->ability_db.array[i]);
			break;
		}
	}
	printf("Learning ability \"Rage\"...\n");
	if (rage_ability) {
		parse_sequence(rage_ability->sequence);
	}
	printf("Learned ability \"Rage\"\n");
	Act* begin_rage_act = NULL;
	Act* end_rage_act = NULL;
	for(int i = 0; i < b->player.acts.size; i++) {
		if (!strcmp("begin-rage", b->player.acts.array[i].private_name)) {
			begin_rage_act = &(b->player.acts.array[i]);
			printf("Learned new bonus action \"Begin Rage\"\n");
		}
		if (!strcmp("end-rage", b->player.acts.array[i].private_name)) {
			end_rage_act = &(b->player.acts.array[i]);
			printf("Learned new bonus action \"End Rage\"\n");
		}
	}
	int* rage_counter_ptr = g_str_int_hash_table_lookup(b->player.arbitrary_counter, "rage-count");
	if (rage_counter_ptr) {
		printf("\"rage-count\" = %d\n", *rage_counter_ptr);
	} else {
		printf("No counter labeled \"rage-count\"\n");
	}
	if (begin_rage_act) {
		printf("Executing \"Begin Rage\"\n");
		parse_sequence(begin_rage_act->sequence);
	}
	else printf("Error with \"Begin Rage\"\n");
	rage_counter_ptr = g_str_int_hash_table_lookup(b->player.arbitrary_counter, "rage-count");
	if (rage_counter_ptr) {
		printf("\"rage-count\" = %d\n", *rage_counter_ptr);
	} else {
		printf("No counter labeled \"rage-count\"\n");
	}
	if (end_rage_act) {
		printf("Executing \"End Rage\"\n");
		parse_sequence(end_rage_act->sequence);
	}
	else printf("Error with \"End Rage\"\n");
	blob_clear();
	return 0;
}
