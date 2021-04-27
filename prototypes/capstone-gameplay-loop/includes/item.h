#ifndef ITEM_H
#define ITEM_H

#include <glib.h>
#include "utils.h"
#include "Dyna.h"

typedef enum {
	ITEM_CATEGORY_STANDARD, ITEM_CATEGORY_ARMOR, ITEM_CATEGORY_WEAPON,
	ITEM_CATEGORY_TOOL, ITEM_CATEGORY_VALUABLE, ITEM_CATEGORY_FOOD,
	ITEM_CATEGORY_CONTAINER, ITEM_CATEGORY_AMMO
} itemCategoryType;

#define ARMOR_FLAG_LIGHT				0x1
#define ARMOR_FLAG_MEDIUM				0x2
#define ARMOR_FLAG_HEAVY				0x4
#define ARMOR_FLAG_UNARMORED			0x8
#define ARMOR_FLAG_SHIELD				0x10
#define ARMOR_FLAG_STEALTH_DISADVANTAGE	0x20

#define WEAPON_FLAG_AMMUNITION			0x1
#define WEAPON_FLAG_FINESSE				0x2
#define WEAPON_FLAG_HEAVY				0x4
#define WEAPON_FLAG_LIGHT				0x8
#define WEAPON_FLAG_LOADING				0x10
#define WEAPON_FLAG_RANGE				0x20
#define WEAPON_FLAG_REACH				0x40
#define WEAPON_FLAG_SPECIAL				0x80
#define WEAPON_FLAG_THROWN				0x100
#define WEAPON_FLAG_TWO_HANDED			0x200
#define WEAPON_FLAG_VERSATILE			0x400
#define WEAPON_FLAG_IMPROVISED			0x800
#define WEAPON_FLAG_UNARMED				0x1000
#define WEAPON_FLAG_MELEE				0x2000
#define WEAPON_FLAG_MAGIC				0x4000
#define WEAPON_FLAG_SIMPLE				0x8000
#define WEAPON_FLAG_MARTIAL				0X10000
#define WEAPON_FLAG_BOW					0x20000
#define WEAPON_FLAG_CROSSBOW			0x40000
#define WEAPON_FLAG_SILVERED			0x80000

#define TOOL_FLAG_ARTISANS_TOOLS		0x1
#define TOOL_FLAG_GAMING_SET			0x2
#define TOOL_FLAG_INSTRUMENT			0x4
#define TOOL_FLAG_LAND_VEHICLE			0x8
#define TOOL_FLAG_WATER_VEHICLE			0x10

#define VALUABLE_FLAG_CURRENCY			0x1
#define VALUABLE_FLAG_GEMSTONE			0x2
#define VALUABLE_FLAG_DECOR				0x4
#define VALUABLE_FLAG_TRADE_GOOD		0x8

#define CONTAINER_FLAG_HOLDS_ARROWS		0x1
#define CONTAINER_FLAG_HOLDS_BOLTS		0x2
#define CONTAINER_FLAG_HOLDS_BULLETS	0x4
#define CONTAINER_FLAG_HOLDS_NEEDLE		0x8
#define CONTAINER_FLAG_HOLDS_PAPER		0x10
#define CONTAINER_FLAG_HOLDS_PARCHMENT	0x10

#define AMMO_FLAG_ARROW					0x1
#define AMMO_FLAG_BOLT					0x2
#define AMMO_FLAG_BULLET				0x4
#define AMMO_FLAG_NEEDLE				0x8

typedef struct {
	int ac;
	int str_req;
	char* don_sequence;	
	char* doff_sequence;	
} Armor;

Armor* armor_init(Armor* rmr);
int armor_clear(Armor*);

typedef struct {
	DamageDyna damage;
	DamageDyna versatile_damage;
	int range;
	int long_range;
	int ammo_type;
	char* equip_sequence;
	char* unequip_sequence;
} Weapon;

Weapon* weapon_init(Weapon* wpn);
int weapon_clear(Weapon*);

typedef struct {
	double max_weight;
	double max_dry_volume;
	double max_wet_volume;
	int quantity; //HOW WOULD YOU ENCODE THE POUCH (20 BULLETS, 50 DARTS)
} Container;

Container* container_init(Container* ctnr);
int container_clear(Container*);

typedef struct {
	char* public_name;
	char* private_name;
	char* source_book;
	int measure_type;			//be it by count, by cubic feet, or by gallons
	itemCategoryType category;
	int flags;
	double value;
	double weight;
	SpellList* spells;
	char* take_sequence;
	char* drop_sequence;
	union {
		Armor* a;
		Weapon* w;
		Container* c;
	} sub;
} ItemDef;

ItemDef* item_def_init(ItemDef*);
int item_def_clear(ItemDef* itm);

generate_dyna_headers_M(ItemDef);

typedef struct __ItemRef ItemRef;

generate_dyna_headers_M(ItemRef);

struct __ItemRef {
	char* private_name;
	char* public_name;
	SpellList* spells;
	double quantity;
};

ItemRef* item_ref_init(ItemRef* itm);

int item_ref_clear(ItemRef* itm);

#endif
