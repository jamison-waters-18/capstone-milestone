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
		if (string_eq(attr_name, "ac")) {
			a->ac = atoi(attr_val);
		}
		else if (string_eq(attr_name, "str-req")) {
			a->str_req = atoi(attr_val);
		}
		else if (string_eq(attr_name, "don-sequence")) {
			a->don_sequence = set_string(a->don_sequence, attr_val);
		}
		else if (string_eq(attr_name, "doff-sequence")) {
			a->doff_sequence = set_string(a->doff_sequence, attr_val);
		}
		else if (string_eq(attr_name, "flags")) {
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
		if (string_eq(attr_name, "range")) {
			sscanf(attr_val, "%d/%d", &(w->range), &(w->long_range));
		}
		else if (string_eq(attr_name, "damage")) {
			parse_damage(&(w->damage), attr_val);
		}
		else if (string_eq(attr_name, "versatile-damage")) {
			parse_damage(&(w->versatile_damage), attr_val);
		}
		else if (string_eq(attr_name, "equip-sequence")) {
			w->equip_sequence = set_string(w->equip_sequence, attr_val);
		}
		else if (string_eq(attr_name, "unequip-sequence")) {
			w->unequip_sequence = set_string(w->unequip_sequence, attr_val);
		}
		else if (string_eq(attr_name, "flags")) {
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
		else if (string_eq(attr_name, "ammo-type")) {
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
		if (string_eq(attr_name, "max-weight")) {
			c->max_weight = atof(attr_val);
		}
		else if (string_eq(attr_name, "max-dry-volume")) {
			c->max_dry_volume = atof(attr_val);
		}
		else if (string_eq(attr_name, "max-wet-volume")) {
			c->max_wet_volume = atof(attr_val);
		}
		else if (string_eq(attr_name, "quantity")) {
			printf("(importing %s) I can't import quantity yet.", item->private_name);
		}
		else if (string_eq(attr_name, "flags")) {
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
		if (string_eq(attr_name, "flags")) {
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
		if (string_eq(attr_name, "flags")) {
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
		if (string_eq(attr_name, "private-name")) {
			itm->private_name = set_string(itm->private_name, attr_val);
		}
		else if (string_eq(attr_name, "public-name")) {
			itm->public_name = set_string(itm->public_name, attr_val);
		}
		else if (string_eq(attr_name, "source-book")) {
			itm->source_book = set_string(itm->source_book, attr_val);
		}
		else if (string_eq(attr_name, "value")) {
			itm->value = atof(attr_val);
		}
		else if (string_eq(attr_name, "weight")) {
			itm->weight = atof(attr_val);
		}
		else if (string_eq(attr_name, "category")) {
			if (string_eq(attr_val, "armor")) {
				itm->category = ITEM_CATEGORY_ARMOR;
				read_armor_xml(node, itm);
			}
			else if (string_eq(attr_val, "weapon")) {
				itm->category = ITEM_CATEGORY_WEAPON;
				read_weapon_xml(node, itm);
			}
			else if (string_eq(attr_val, "tool")) {
				itm->category = ITEM_CATEGORY_TOOL;
				read_tool_xml(node, itm);
			}
			else if (string_eq(attr_val, "valuable")) {
				itm->category = ITEM_CATEGORY_VALUABLE;
				read_valuable_xml(node, itm);
			}
			else if (string_eq(attr_val, "food")) {
				itm->category = ITEM_CATEGORY_FOOD;
			}
			else if (string_eq(attr_val, "container")) {
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
		while(kid) {
			read_item_xml(kid);
			kid = mxmlGetNextSibling(kid);
		}
	}
	else if (string_eq(element_name, "item-def")) {
		ItemDef itm;
		item_def_init(&itm);
		ItemDefDyna_add(&(get_blob()->item_db), *(read_item_def_xml(&itm, node)));
	}
	else if (string_eq(element_name, "item-bundle")) {
		//printf("I can't handle 'item-bundle' yet \n");
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
		while(kid) {
			read_ability_xml(kid);
			kid = mxmlGetNextSibling(kid);
		}
	}
	else if (string_eq(element_name, "ability-def")) {
		AbilityDef ablt;
		ability_def_init(&ablt);
		int num_attrs = mxmlElementGetAttrCount(node);
		for (int i = 0; i < num_attrs; i++) {
			const char* attr_name;
			const char* attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
			if (string_eq(attr_name, "private-name")) {
				ablt.private_name = set_string(ablt.private_name, attr_val);
			}
			else if (string_eq(attr_name, "public-name")) {
				ablt.public_name = set_string(ablt.public_name, attr_val);
			}
			else if (string_eq(attr_name, "source-book")) {
				ablt.source_book = set_string(ablt.source_book, attr_val);
			}
			else if (string_eq(attr_name, "prereqs")) {
				ablt.prereqs = set_string(ablt.prereqs, attr_val);
			}
			else if (string_eq(attr_name, "sequence")) {
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














ItemRef read_item_ref_xml(mxml_node_t* node) {
	ItemRef itm;
	item_ref_init(&itm);
	if (!node) { return itm; }
	const char* element_name = mxmlGetElement(node);
	if (!element_name) { return itm; }
	if (strcmp(element_name, "item-ref")) return itm;
	int num_attrs = mxmlElementGetAttrCount(node);
	const char* attr_name;
	const char* attr_val;
	for (int i = 0; i < num_attrs; i++) {
		attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
		if (string_eq(attr_name, "private-name")) {
			itm.private_name = set_string(itm.private_name, attr_val);
		} else if (string_eq(attr_name, "public-name")) {
			itm.public_name = set_string(itm.public_name, attr_val);
		} else if (string_eq(attr_name, "number")) {
			itm.quantity = atof(attr_val);
			if (!itm.quantity) {
				printf("Items shouldn't have 0 quantity\n");
			}
		} else if (string_eq(attr_name, "spell-list")) {
			//TODO: let items contain spells
			printf("Items can carry spells once spells have been invented\n");
		} else {
			printf("Items shouldn't have property %s\n", attr_name);
		}
	}

	if (!itm.public_name && itm.private_name) {
		ItemDefDyna* defs = &(get_blob()->item_db);
		for (int i = 0; i < defs->size; i++) {
			if (string_eq(itm.private_name, defs->array[i].private_name)) {
				itm.public_name = set_string(NULL, defs->array[i].public_name);
				break;
			}
		}
	}

	return itm;
}

void read_character_xml_children(PlayerCharacter* player, mxml_node_t* node) {
	if (!node) { return; }
	const char* element_name = mxmlGetElement(node);
	if (!element_name) { return; }
	if(strstr(element_name, "!--") == element_name) { return; }
	int num_attrs = mxmlElementGetAttrCount(node);
	const char* attr_name;
	const char* attr_val;
	if (string_eq(element_name, "class")) {
		ClassRole c;
		class_role_init(&c);
		for (int i = 0; i < num_attrs; i++) {
			attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
			if (string_eq(attr_name, "private-name")) {
				c.private_name = set_string(c.private_name, attr_val);
			} else if (string_eq(attr_name, "public-name")) {
				c.public_name = set_string(c.public_name, attr_val);
			} else if (string_eq(attr_name, "level")) {
				c.level = atoi(attr_val);
			} else if (string_eq(attr_name, "hit-die-size")) {
				c.hit_die_size = atoi(attr_val);
			} else if (string_eq(attr_name, "hp-per-level")) {
				c.given_hp_per_level = set_string(c.given_hp_per_level, attr_val);
			} else if (string_eq(attr_name, "subclass")) {
				c.subclass = set_string(c.subclass, attr_val);
			} else {
				printf("Items shouldn't have property %s\n", attr_name);
			}
		}
		ClassRoleDyna_add(&(player->classes), c);
	} else if (string_eq(element_name, "armor")) {
		mxml_node_t* kid = mxmlGetFirstChild(node);
		while(kid) {
			ItemRef itm = read_item_ref_xml(kid);
			if (itm.private_name) {
				item_ref_clear(&(player->armor));
				player->armor = itm;
			}
			kid = mxmlGetNextSibling(kid);
		}
	} else if (string_eq(element_name, "main-hand")) {
		mxml_node_t* kid = mxmlGetFirstChild(node);
		while(kid) {
			ItemRef itm = read_item_ref_xml(kid);
			if (itm.private_name) {
				item_ref_clear(&(player->main_hand));
				player->main_hand = itm;
			}
			kid = mxmlGetNextSibling(kid);
		}
	} else if (string_eq(element_name, "off-hand")) {
		mxml_node_t* kid = mxmlGetFirstChild(node);
		while(kid) {
			ItemRef itm = read_item_ref_xml(kid);
			if (itm.private_name) {
				item_ref_clear(&(player->off_hand));
				player->off_hand = itm;
			}
			kid = mxmlGetNextSibling(kid);
		}
	} else if (string_eq(element_name, "inventory")) {
		mxml_node_t* kid = mxmlGetFirstChild(node);
		while(kid) {
			ItemRef itm = read_item_ref_xml(kid);
			if (itm.private_name) {
				ItemRefDyna_add(&(player->inventory), itm);
			}
			kid = mxmlGetNextSibling(kid);
		}
	} else if (string_eq(element_name, "skill-proficiency")) {
		for (int i = 0; i < num_attrs; i++) {
			attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
			if (string_eq(attr_name, "private-name")) {
				if (string_eq(attr_val, "athletics")) {
					player->skill_proficiencies[(int) ATHLETICS] += 1;
				}
				else if (string_eq(attr_val, "acrobatics")) {
					player->skill_proficiencies[(int) ACROBATICS] += 1;
				}
				else if (string_eq(attr_val, "sleight-of-hand")) {
					player->skill_proficiencies[(int) SLEIGHT_OF_HAND] += 1;
				}
				else if (string_eq(attr_val, "stealth")) {
					player->skill_proficiencies[(int) STEALTH] += 1;
				}
				else if (string_eq(attr_val, "arcana")) {
					player->skill_proficiencies[(int) ARCANA] += 1;
				}
				else if (string_eq(attr_val, "history")) {
					player->skill_proficiencies[(int) HISTORY] += 1;
				}
				else if (string_eq(attr_val, "investigation")) {
					player->skill_proficiencies[(int) INVESTIGATION] += 1;
				}
				else if (string_eq(attr_val, "nature")) {
					player->skill_proficiencies[(int) NATURE] += 1;
				}
				else if (string_eq(attr_val, "religion")) {
					player->skill_proficiencies[(int) RELIGION] += 1;
				}
				else if (string_eq(attr_val, "animal-handling")) {
					player->skill_proficiencies[(int) ANIMAL_HANDLING] += 1;
				}
				else if (string_eq(attr_val, "insight")) {
					player->skill_proficiencies[(int) INSIGHT] += 1;
				}
				else if (string_eq(attr_val, "medicine")) {
					player->skill_proficiencies[(int) MEDICINE] += 1;
				}
				else if (string_eq(attr_val, "perception")) {
					player->skill_proficiencies[(int) PERCEPTION] += 1;
				}
				else if (string_eq(attr_val, "survival")) {
					player->skill_proficiencies[(int) SURVIVAL] += 1;
				}
				else if (string_eq(attr_val, "deception")) {
					player->skill_proficiencies[(int) DECEPTION] += 1;
				}
				else if (string_eq(attr_val, "intimidation")) {
					player->skill_proficiencies[(int) INTIMIDATION] += 1;
				}
				else if (string_eq(attr_val, "performance")) {
					player->skill_proficiencies[(int) PERFORMANCE] += 1;
				}
				else if (string_eq(attr_val, "persuasion")) {
					player->skill_proficiencies[(int) PERSUASION] += 1;
				}
				else {
					printf("Unknown skill %s\n", attr_val);
				}
			}
		}
	} else if (string_eq(element_name, "item-proficiency")) {
		for (int i = 0; i < num_attrs; i++) {
			attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
			if (string_eq(attr_name, "private-name")) {
				g_ptr_array_insert(player->item_proficiencies, -1, 
						set_string(NULL, attr_val));
			} else {
				printf("Item proficiency shouldn't have property %s\n", attr_name);
			}
		}
	} else if (string_eq(element_name, "item-category-proficiency")) {
		ItemDef itm;
		item_def_init(&itm);
		read_item_def_xml(&itm, node);
		ItemCategory ic;
		ic.category = itm.category;
		ic.flags = itm.flags;
		item_def_clear(&itm);
		ItemCategoryDyna_add(&(player->item_category_proficiencies), ic);
	} else if (string_eq(element_name, "language")) {
		for (int i = 0; i < num_attrs; i++) {
			attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
			if (string_eq(attr_name, "private-name")) {
				g_ptr_array_insert(player->languages, -1, 
						set_string(NULL, attr_val));
			} else {
				printf("Languages shouldn't have property %s\n", attr_name);
			}
		}
	} else if (string_eq(element_name, "dormant-ability")) {
		AbilityRef abr;
		ability_ref_init(&abr);
		for (int i = 0; i < num_attrs; i++) {
			attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
			if (string_eq(attr_name, "private-name")) {
				abr.private_name = set_string(abr.private_name, attr_val);
			} else {
				printf("Ability references shouldn't have property %s\n", attr_name);
			}
		}
		AbilityRefDyna_add(&(player->dormant_abilities), abr);
	} else if (string_eq(element_name, "active-ability")) {
		AbilityRef abr;
		ability_ref_init(&abr);
		for (int i = 0; i < num_attrs; i++) {
			attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
			if (string_eq(attr_name, "private-name")) {
				abr.private_name = set_string(abr.private_name, attr_val);
			} else {
				printf("Ability references shouldn't have property %s\n", attr_name);
			}
		}
		AbilityRefDyna_add(&(player->active_abilities), abr);
	} else {
		printf("Character XML Child Error: unknown trait %s\n", element_name);
	}
}

void read_character_xml(PlayerCharacter* player, mxml_node_t* node) {
	if (!node) { return; }
	const char* element_name = mxmlGetElement(node);
	if (!element_name) { return; }
	if (strstr(element_name, "xml")) {
		mxml_node_t* kid = mxmlGetFirstChild(node);
		while(kid) {
			read_character_xml(player, kid);
			kid = mxmlGetNextSibling(kid);
		}
	}
	else if (string_eq(element_name, "player-character")) {
		int num_attrs = mxmlElementGetAttrCount(node);
		for (int i = 0; i < num_attrs; i++) {
			const char* attr_name;
			const char* attr_val = mxmlElementGetAttrByIndex(node, i, &attr_name);
			if (string_eq(attr_name, "private-name")) {
				player->private_name = set_string(player->private_name, attr_val);
			}
			else if (string_eq(attr_name, "public-name")) {
				player->public_name = set_string(player->public_name, attr_val);
			}
			else if (string_eq(attr_name, "race")) {
				player->race = set_string(player->race, attr_val);
			}
			else if (string_eq(attr_name, "subrace")) {
				player->subrace = set_string(player->subrace, attr_val);
			}
			else if (string_eq(attr_name, "xp")) {
				player->xp = atoi(attr_val);
			}
			else if (string_eq(attr_name, "hp")) {
				player->hp = atoi(attr_val);
			}
			else if (string_eq(attr_name, "chosen-hp-max")) {
				player->chosen_hp_max = atoi(attr_val);
			}
			else if (string_eq(attr_name, "thp")) {
				player->thp = atoi(attr_val);
			}
			else if (string_eq(attr_name, "hit-dice")) {
				int num = 0; int die = 0;
				int count = sscanf(attr_val, "%dd%d", &num, &die);
				if (count == 2) {
					player->hit_dice.die[dicenum_to_index(die)] = num;
				}
			}
			else if (string_eq(attr_name, "inspiration-dice")) {
				int num = 0; int die = 0;
				int count = sscanf(attr_val, "%dd%d", &num, &die);
				if (count == 2) {
					player->inspiration_die = die;
				}
			}
			else if (string_eq(attr_name, "valor-inspiration")) {
				if (string_eq(attr_val, "true")) {
					player->valor_inspiration = 1;
				}
			}
			else if (string_eq(attr_name, "verbal-limits")) {
				player->verbal_limits = atoi(attr_val);
			}
			else if (string_eq(attr_name, "somatic-limits")) {
				player->somatic_limits = atoi(attr_val);
			}
			else if (string_eq(attr_name, "concentration-limits")) {
				player->concentration_limits = atoi(attr_val);
			}
			else if (string_eq(attr_name, "ac-bonus")) {
				player->ac_bonus = atoi(attr_val);
			}
			else if (string_eq(attr_name, "exhaustion")) {
				player->exhaustion = atoi(attr_val);
			}
			else if (string_eq(attr_name, "speed")) {
				player->walk_speed = atoi(attr_val);
			}
			else if (string_eq(attr_name, "str")) {
				player->ability_scores[0] = atoi(attr_val);
			}
			else if (string_eq(attr_name, "dex")) {
				player->ability_scores[1] = atoi(attr_val);
			}
			else if (string_eq(attr_name, "con")) {
				player->ability_scores[2] = atoi(attr_val);
			}
			else if (string_eq(attr_name, "int")) {
				player->ability_scores[3] = atoi(attr_val);
			}
			else if (string_eq(attr_name, "wis")) {
				player->ability_scores[4] = atoi(attr_val);
			}
			else if (string_eq(attr_name, "cha")) {
				player->ability_scores[5] = atoi(attr_val);
			}
			else if (string_eq(attr_name, "size")) {
				player->size = str_to_size(attr_val);
			}
			else if (string_eq(attr_name, "alignment")) {
				player->alignment = str_to_alignment(attr_val);
			}
		}
		mxml_node_t* kid = mxmlGetFirstChild(node);
		while(kid) {
			read_character_xml_children(player, kid);
			kid = mxmlGetNextSibling(kid);
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
