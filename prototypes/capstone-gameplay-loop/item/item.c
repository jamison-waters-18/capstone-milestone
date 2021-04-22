#include "../includes/item.h"
#include "../includes/DynaImp.h"
#include "../includes/glib-facade.h"

generate_dyna_functions_M(ItemDef);

Armor* armor_init(Armor* rmr) {
	if (!rmr) { return NULL; }
	rmr->ac = 0;
	rmr->str_req = 0;
	rmr->don_sequence = NULL;
	rmr->doff_sequence = NULL;
	return rmr;
}

int armor_clear(Armor* rmr) {
	if (!rmr) { return -1; }
	free(rmr->don_sequence);
	free(rmr->doff_sequence);
	return 0;
}

Weapon* weapon_init(Weapon* wpn) {
	if (!wpn) { return NULL; }
	DamageDyna_init(&(wpn->damage));
	DamageDyna_init(&(wpn->versatile_damage));
	wpn->range = 0;
	wpn->long_range = 0;
	wpn->ammo_type = 0;
	wpn->equip_sequence = NULL;
	wpn->unequip_sequence = NULL;
	return wpn;
}

int weapon_clear(Weapon* wpn) {
	if (!wpn) { return -1; }
	DamageDyna_clear(&(wpn->damage), NULL);
	DamageDyna_clear(&(wpn->versatile_damage), NULL);
	free(wpn->equip_sequence);
	free(wpn->unequip_sequence);
	return 0;
}

Container* container_init(Container* ctnr) {
	if (!ctnr) { return NULL; }
	ctnr->max_weight = 0.0;
	ctnr->max_dry_volume = 0.0;
	ctnr->max_wet_volume = 0.0;
	ctnr->quantity = 0;
	return ctnr;
}

int container_clear(Container* ctnr) {
	if (!ctnr) { return -1; }
	return 0;
}

ItemDef* item_def_init(ItemDef* itm) {
	if (!itm) { return NULL; }
	itm->public_name = NULL;
	itm->private_name = NULL;
	itm->source_book = set_string(NULL, "players-handbook");
	itm->take_sequence = NULL;
	itm->drop_sequence = NULL;
	itm->measure_type = 0;
	itm->flags = 0;
	itm->category = ITEM_CATEGORY_STANDARD;
	itm->value = 0.0;
	itm->weight = 0.0;
	itm->spells = NULL;
	itm->sub.a = NULL;
	return itm;
}

int item_def_clear(ItemDef* itm) {
	if (!itm) { return -1; }
	free(itm->public_name);
	free(itm->private_name);
	free(itm->source_book);
	free(itm->take_sequence);
	free(itm->drop_sequence);
	free(itm->spells);
	itm->spells = NULL;
	switch (itm->category) {
	case ITEM_CATEGORY_ARMOR:
		armor_clear(itm->sub.a);
		free(itm->sub.a);
		break;
	case ITEM_CATEGORY_WEAPON:
		weapon_clear(itm->sub.w);
		free(itm->sub.w);
		break;
	case ITEM_CATEGORY_CONTAINER:
		container_clear(itm->sub.c);
		free(itm->sub.c);
		break;
	default: break;
	}
	return 0;
}

generate_dyna_functions_M(ItemRef);

ItemRef* item_ref_init(ItemRef* itm) {
	if (!itm) { return NULL; }
	itm->private_name = NULL;
	itm->quantity = 1.0;
	itm->spells = NULL;
	return itm;
}

int item_ref_clear(ItemRef* itm) {
	if (!itm) { return -1; }
	free(itm->private_name);
	free(itm->spells);
	return 0;
}
