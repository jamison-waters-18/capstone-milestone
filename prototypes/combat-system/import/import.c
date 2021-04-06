#include <mxml.h>
#include <glib.h>
#include <dirent.h>
#include <string.h>
#include "../includes/import.h"
#include "../includes/blob.h"

void read_armor_xml(mxml_node_t* node, ItemDef* item) {
	Armor* a = malloc(sizeof(Armor));
	armor_init(a);
	int num_attrs = mxmlElementGetAttrCount(node);
	for (int i = 0; i < num_attrs; i++) {
		const char* attr_name;
		const char* attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
		if (!strcmp(attr_name, "ac")) {
			a->ac = atoi(attr_val);
		}
		else if (!strcmp(attr_name, "str-req")) {
			a->str_req = atoi(attr_val);
		}
		else if (!strcmp(attr_name, "don-sequence")) {
			a->don_sequence = set_string(a->don_sequence, attr_val);
		}
		else if (!strcmp(attr_name, "doff-sequence")) {
			a->doff_sequence = set_string(a->doff_sequence, attr_val);
		}
		else if (!strcmp(attr_name, "flags")) {
			if (strstr(attr_val, "light")) {
				item->flags |= ARMOR_FLAG_LIGHT;
			}
			if (strstr(attr_val, "medium")) {
				item->flags |= ARMOR_FLAG_MEDIUM;
			}
			if (strstr(attr_val, "heavy")) {
				item->flags |= ARMOR_FLAG_HEAVY;
			}
			if (strstr(attr_val, "unarmored")) {
				item->flags |= ARMOR_FLAG_UNARMORED;
			}
			if (strstr(attr_val, "shield")) {
				item->flags |= ARMOR_FLAG_SHIELD;
			}
			if (strstr(attr_val, "stealth disadvantage")) {
				item->flags |= ARMOR_FLAG_STEALTH_DISADVANTAGE;
			}
		}
	}
	item->sub.a = a;
}

void read_weapon_xml(mxml_node_t* node, ItemDef* item) {
	Weapon* w = malloc(sizeof(Weapon));
	weapon_init(w);
	int num_attrs = mxmlElementGetAttrCount(node);
	for (int i = 0; i < num_attrs; i++) {
		const char* attr_name;
		const char* attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
		if (!strcmp(attr_name, "range")) {
			w->range = atoi(attr_val);
		}
		else if (!strcmp(attr_name, "long-range")) {
			w->long_range = atoi(attr_val);
		}
		else if (!strcmp(attr_name, "long-range")) {
			w->long_range = atoi(attr_val);
		}
		else if (!strcmp(attr_name, "damage")) {
			parse_damage(&(w->damage), attr_val);
		}
		else if (!strcmp(attr_name, "versatile-damage")) {
			parse_damage(&(w->versatile_damage), attr_val);
		}
		else if (!strcmp(attr_name, "equip-sequence")) {
			w->equip_sequence = set_string(w->equip_sequence, attr_val);
		}
		else if (!strcmp(attr_name, "unequip-sequence")) {
			w->unequip_sequence = set_string(w->unequip_sequence, attr_val);
		}
		else if (!strcmp(attr_name, "flags")) {
			if (strstr(attr_val, "ammunition")) {
				item->flags |= WEAPON_FLAG_AMMUNITION;
			}
			if (strstr(attr_val, "finesse")) {
				item->flags |= WEAPON_FLAG_FINESSE;
			}
			if (strstr(attr_val, "heavy")) {
				item->flags |= WEAPON_FLAG_HEAVY;
			}
			if (strstr(attr_val, "light")) {
				item->flags |= WEAPON_FLAG_LIGHT;
			}
			if (strstr(attr_val, "loading")) {
				item->flags |= WEAPON_FLAG_LOADING;
			}
			if (strstr(attr_val, "range")) {
				item->flags |= WEAPON_FLAG_RANGE;
			}
			if (strstr(attr_val, "reach")) {
				item->flags |= WEAPON_FLAG_REACH;
			}
			if (strstr(attr_val, "special")) {
				item->flags |= WEAPON_FLAG_SPECIAL;
			}
			if (strstr(attr_val, "thrown")) {
				item->flags |= WEAPON_FLAG_THROWN;
			}
			if (strstr(attr_val, "two-handed")) {
				item->flags |= WEAPON_FLAG_TWO_HANDED;
			}
			if (strstr(attr_val, "versatile")) {
				item->flags |= WEAPON_FLAG_VERSATILE;
			}
			if (strstr(attr_val, "improvised")) {
				item->flags |= WEAPON_FLAG_IMPROVISED;
			}
			if (strstr(attr_val, "unarmed")) {
				item->flags |= WEAPON_FLAG_UNARMED;
			}
			if (strstr(attr_val, "melee")) {
				item->flags |= WEAPON_FLAG_MELEE;
			}
			if (strstr(attr_val, "magic")) {
				item->flags |= WEAPON_FLAG_MAGIC;
			}
			if (strstr(attr_val, "simple")) {
				item->flags |= WEAPON_FLAG_SIMPLE;
			}
			if (strstr(attr_val, "martial")) {
				item->flags |= WEAPON_FLAG_MARTIAL;
			}
			if (strstr(attr_val, "bow") && !strstr(attr_val, "crossbow")) {
				item->flags |= WEAPON_FLAG_BOW;
			}
			if (strstr(attr_val, "crossbow")) {
				item->flags |= WEAPON_FLAG_CROSSBOW;
			}
			if (strstr(attr_val, "silvered")) {
				item->flags |= WEAPON_FLAG_SILVERED;
			}
		}
		else if (!strcmp(attr_name, "ammo-type")) {
			if (strstr(attr_val, "arrow")) {
				item->flags |= AMMO_FLAG_ARROW;
			}
			if (strstr(attr_val, "bolt")) {
				item->flags |= AMMO_FLAG_BOLT;
			}
			if (strstr(attr_val, "bullet")) {
				item->flags |= AMMO_FLAG_BULLET;
			}
			if (strstr(attr_val, "needle")) {
				item->flags |= AMMO_FLAG_NEEDLE;
			}
		}
	}
	item->sub.w = w;
}

void read_container_xml(mxml_node_t* node, ItemDef* item) {
	Container* c = malloc(sizeof(Container));
	container_init(c);
	int num_attrs = mxmlElementGetAttrCount(node);
	for (int i = 0; i < num_attrs; i++) {
		const char* attr_name;
		const char* attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
		if (!strcmp(attr_name, "max-weight")) {
			c->max_weight = atof(attr_val);
		}
		else if (!strcmp(attr_name, "max-dry-volume")) {
			c->max_dry_volume = atof(attr_val);
		}
		else if (!strcmp(attr_name, "max-wet-volume")) {
			c->max_wet_volume = atof(attr_val);
		}
		else if (!strcmp(attr_name, "quantity")) {
			printf("(importing %s) I can't import quantity yet.", item->private_name);
		}
		else if (!strcmp(attr_name, "flags")) {
			if (strstr(attr_val, "light")) {
				item->flags |= ARMOR_FLAG_LIGHT;
			}
			if (strstr(attr_val, "medium")) {
				item->flags |= ARMOR_FLAG_LIGHT;
			}
			if (strstr(attr_val, "heavy")) {
				item->flags |= ARMOR_FLAG_HEAVY;
			}
			if (strstr(attr_val, "unarmored")) {
				item->flags |= ARMOR_FLAG_UNARMORED;
			}
			if (strstr(attr_val, "shield")) {
				item->flags |= ARMOR_FLAG_SHIELD;
			}
			if (strstr(attr_val, "stealth disadvantage")) {
				item->flags |= ARMOR_FLAG_STEALTH_DISADVANTAGE;
			}
		}
	}
	item->sub.c = c;
}

void read_tool_xml(mxml_node_t* node, ItemDef* item) {
	int num_attrs = mxmlElementGetAttrCount(node);
	for (int i = 0; i < num_attrs; i++) {
		const char* attr_name;
		const char* attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
		if (!strcmp(attr_name, "flags")) {
			if (strstr(attr_val, "artisan's tools")) {
				item->flags |= TOOL_FLAG_ARTISANS_TOOLS;
			}
			if (strstr(attr_val, "gaming set")) {
				item->flags |= TOOL_FLAG_GAMING_SET;
			}
			if (strstr(attr_val, "instrument")) {
				item->flags |= TOOL_FLAG_INSTRUMENT;
			}
			if (strstr(attr_val, "land vehicle")) {
				item->flags |= TOOL_FLAG_LAND_VEHICLE;
			}
			if (strstr(attr_val, "water vehicle")) {
				item->flags |= TOOL_FLAG_WATER_VEHICLE;
			}
		}
	}
}

void read_valuable_xml(mxml_node_t* node, ItemDef* item) {
	int num_attrs = mxmlElementGetAttrCount(node);
	for (int i = 0; i < num_attrs; i++) {
		const char* attr_name;
		const char* attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
		if (!strcmp(attr_name, "flags")) {
			if (strstr(attr_val, "currency")) {
				item->flags |= VALUABLE_FLAG_CURRENCY;
			}
			if (strstr(attr_val, "gem")) {
				item->flags |= VALUABLE_FLAG_GEMSTONE;
			}
			if (strstr(attr_val, "decor")) {
				item->flags |= VALUABLE_FLAG_DECOR;
			}
			if (strstr(attr_val, "trade good")) {
				item->flags |= VALUABLE_FLAG_TRADE_GOOD;
			}
		}
	}
}

ItemDef* read_item_def_xml(ItemDef* itm, mxml_node_t* node) {
	int num_attrs = mxmlElementGetAttrCount(node);
	for (int i = 0; i < num_attrs; i++) {
		const char* attr_name;
		const char* attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
		if (!strcmp(attr_name, "private-name")) {
			itm->private_name = set_string(itm->private_name, attr_val);
		}
		else if (!strcmp(attr_name, "public-name")) {
			itm->public_name = set_string(itm->public_name, attr_val);
		}
		else if (!strcmp(attr_name, "source-book")) {
			itm->source_book = set_string(itm->source_book, attr_val);
		}
		else if (!strcmp(attr_name, "value")) {
			itm->value = atof(attr_val);
		}
		else if (!strcmp(attr_name, "weight")) {
			itm->weight = atof(attr_val);
		}
		else if (!strcmp(attr_name, "category")) {
			if (!strcmp(attr_val, "armor")) {
				itm->category = ITEM_CATEGORY_ARMOR;
				read_armor_xml(node, itm);
			}
			else if (!strcmp(attr_val, "weapon")) {
				itm->category = ITEM_CATEGORY_WEAPON;
				read_weapon_xml(node, itm);
			}
			else if (!strcmp(attr_val, "tool")) {
				itm->category = ITEM_CATEGORY_TOOL;
				read_tool_xml(node, itm);
			}
			else if (!strcmp(attr_val, "valuable")) {
				itm->category = ITEM_CATEGORY_VALUABLE;
				read_valuable_xml(node, itm);
			}
			else if (!strcmp(attr_val, "food")) {
				itm->category = ITEM_CATEGORY_FOOD;
			}
			else if (!strcmp(attr_val, "container")) {
				itm->category = ITEM_CATEGORY_CONTAINER;
				read_container_xml(node, itm);
			}
			else {
				printf("Unrecognized category %s\n", attr_val);
			}
		}
	}
	return itm;
}

void read_item_xml(mxml_node_t* node) {
	if (!node) { return; }
	const char* element_name = mxmlGetElement(node);
	if (!element_name) { return; }
	if (strstr(element_name, "xml")) {
		mxml_node_t* kid = mxmlGetFirstChild(node);
		if (kid) {
			while(kid) {
				read_item_xml(kid);
				kid = mxmlGetNextSibling(kid);
			}
		}
	}
	else if (!strcmp(element_name, "item-def")) {
		ItemDef itm;
		item_def_init(&itm);
		ItemDefDyna_add(&(get_blob()->item_db), *(read_item_def_xml(&itm, node)));
	}
	else if (!strcmp(element_name, "item-bundle")) {
		printf("I can't handle 'item-bundle' yet \n");
	}
	else {
		printf("I don't know what %s is\n", element_name);
	}

}

void import_item_file(char* filename) {
	char* file_dir = set_string(NULL, "data/items/");
	file_dir = append_string(file_dir, filename);
	FILE* file = fopen(file_dir, "r");
	free(file_dir);
	if (!file) { return; }
	mxml_node_t* tree = mxmlLoadFile(NULL, file, MXML_TEXT_CALLBACK);
	fclose(file);
	read_item_xml(tree);
	mxmlDelete(tree);
}

void import_items() {
	DIR* folder;
	struct dirent* ent;
	if ((folder = opendir("data/items")) != NULL) {
		while ((ent = readdir (folder)) != NULL) {
			if (strstr(ent->d_name, ".xml")) {
				import_item_file(ent->d_name);
			}
		}
		closedir (folder);
	}
}

























void read_ability_xml(mxml_node_t* node) {
	if (!node) { return; }
	const char* element_name = mxmlGetElement(node);
	if (!element_name) { return; }
	if (strstr(element_name, "xml")) {
		mxml_node_t* kid = mxmlGetFirstChild(node);
		if (kid) {
			while(kid) {
				read_ability_xml(kid);
				kid = mxmlGetNextSibling(kid);
			}
		}
	}
	else if (!strcmp(element_name, "ability-def")) {
		AbilityDef ablt;
		ability_def_init(&ablt);
		int num_attrs = mxmlElementGetAttrCount(node);
		for (int i = 0; i < num_attrs; i++) {
			const char* attr_name;
			const char* attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
			if (!strcmp(attr_name, "private-name")) {
				ablt.private_name = set_string(ablt.private_name, attr_val);
			}
			else if (!strcmp(attr_name, "public-name")) {
				ablt.public_name = set_string(ablt.public_name, attr_val);
			}
			else if (!strcmp(attr_name, "source-book")) {
				ablt.source_book = set_string(ablt.source_book, attr_val);
			}
			else if (!strcmp(attr_name, "prereqs")) {
				ablt.prereqs = set_string(ablt.prereqs, attr_val);
			}
			else if (!strcmp(attr_name, "sequence")) {
				ablt.sequence = set_string(ablt.sequence, attr_val);
			}
		}
		AbilityDefDyna_add(&(get_blob()->ability_db), ablt);
	}
	else {
		printf("I don't know about %s abilities\n", element_name);
	}

}

void import_ability_file(char* filename) {
	char* file_dir = set_string(NULL, "data/abilities/");
	file_dir = append_string(file_dir, filename);
	FILE* file = fopen(file_dir, "r");
	free(file_dir);
	if (!file) { return; }
	mxml_node_t* tree = mxmlLoadFile(NULL, file, MXML_TEXT_CALLBACK);
	fclose(file);
	read_ability_xml(tree);
	mxmlDelete(tree);
}

void import_abilities() {
	DIR* folder;
	struct dirent* ent;
	if ((folder = opendir("data/abilities")) != NULL) {
		while ((ent = readdir (folder)) != NULL) {
			if (strstr(ent->d_name, ".xml")) {
				import_ability_file(ent->d_name);
			}
		}
		closedir (folder);
	}
}
















void read_character_xml_children(PlayerCharacter* player, mxml_node_t* node) {
	if (!node) { return; }
	const char* element_name = mxmlGetElement(node);
	if (!element_name) { return; }
}

void read_character_xml(PlayerCharacter* player, mxml_node_t* node) {
	if (!node) { return; }
	const char* element_name = mxmlGetElement(node);
	if (!element_name) { return; }
	if (strstr(element_name, "xml")) {
		mxml_node_t* kid = mxmlGetFirstChild(node);
		if (kid) {
			while(kid) {
				read_character_xml(player, kid);
				kid = mxmlGetNextSibling(kid);
			}
		}
	}
	else if (!strcmp(element_name, "player-character")) {
		int num_attrs = mxmlElementGetAttrCount(node);
		for (int i = 0; i < num_attrs; i++) {
			const char* attr_name;
			const char* attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
			if (!strcmp(attr_name, "private-name")) {
				player->private_name = set_string(player->private_name, attr_val);
			}
			else if (!strcmp(attr_name, "public-name")) {
				player->public_name = set_string(player->public_name, attr_val);
			}
			else if (!strcmp(attr_name, "race")) {
				player->race = set_string(player->race, attr_val);
			}
			else if (!strcmp(attr_name, "subrace")) {
				player->subrace = set_string(player->subrace, attr_val);
			}
			else if (!strcmp(attr_name, "xp")) {
				player->xp = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "hp")) {
				player->hp = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "chosen-hp-max")) {
				player->chosen_hp_max = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "thp")) {
				player->thp = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "hit-dice")) {
				int num = 0; int die = 0;
				int count = sscanf(attr_val, "%dd%d", &num, &die);
				if (count == 2) {
					player->hit_dice.die[dicenum_to_index(die)] = num;
				}
			}
			else if (!strcmp(attr_name, "inspiration-dice")) {
				int num = 0; int die = 0;
				int count = sscanf(attr_val, "%dd%d", &num, &die);
				if (count == 2) {
					player->inspiration_die = die;
				}
			}
			else if (!strcmp(attr_name, "valor-inspiration")) {
				if (!strcmp(attr_val, "true")) {
					player->valor_inspiration = 1;
				}
			}
			else if (!strcmp(attr_name, "verbal-limits")) {
				player->verbal_limits = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "somatic-limits")) {
				player->somatic_limits = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "concentration-limits")) {
				player->concentration_limits = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "ac-bonus")) {
				player->ac_bonus = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "exhaustion")) {
				player->exhaustion = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "speed")) {
				player->walk_speed = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "str")) {
				player->ability_scores[0] = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "dex")) {
				player->ability_scores[1] = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "con")) {
				player->ability_scores[2] = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "int")) {
				player->ability_scores[3] = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "wis")) {
				player->ability_scores[4] = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "cha")) {
				player->ability_scores[5] = atoi(attr_val);
			}
			else if (!strcmp(attr_name, "size")) {
				player->size = str_to_size(attr_val);
			}
			else if (!strcmp(attr_name, "alignment")) {
				player->alignment = str_to_alignment(attr_val);
			}
		}
		mxml_node_t* kid = mxmlGetFirstChild(node);
		if (kid) {
			while(kid) {
				read_character_xml_children(player, kid);
				kid = mxmlGetNextSibling(kid);
			}
		}
	}
	else {
		printf("I don't know about %s characters\n", element_name);
	}
}

void import_character(char* name) {
	if (!name) { return; }
	char* file_dir = set_string(NULL, "data/saves/");
	file_dir = append_string(file_dir, name);
	file_dir = append_string(file_dir, ".xml");
	FILE* file = fopen(file_dir, "r");
	free(file_dir);
	if (!file) { return; }
	mxml_node_t* tree = mxmlLoadFile(NULL, file, MXML_TEXT_CALLBACK);
	fclose(file);
	read_character_xml(&(get_blob()->player), tree);
	mxmlDelete(tree);
}
