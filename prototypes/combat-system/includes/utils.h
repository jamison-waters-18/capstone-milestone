#ifndef UTILS_H
#define UTILS_H

#include <glib.h>
#include "Dyna.h"

#define SUCCESSFUL_EMPTY_FUNC_EXEC (void*) 1
#define FAILED_FUNC_EXEC 0
#define FLOAT_EQ_TOLERANCE .000001

typedef enum {
	UNALIGNED, LAWFUL_GOOD,    NEUTRAL_GOOD, CHAOTIC_GOOD,
               LAWFUL_NEUTRAL, TRUE_NEUTRAL, CHAOTIC_NEUTRAL,
               LAWFUL_EVIL,    NEUTRAL_EVIL, CHAOTIC_EVIL
} alignmentType;

typedef enum  {
	ABERRATION, BEAST, CELESTIAL, CONSTRUCT, DRAGON, ELEMENTAL,
	FEY, FIEND, GIANT, MONSTROSITY, OOZE, PLANT, UNDEAD
} creatureType;

typedef enum {
	TINY, SMALL, MEDIUM, LARGE, HUGE, GARGANTUAN
} sizeType;

typedef enum{
	ACID, BLUDGEONING, COLD, FIRE, FORCE, 
	LIGHTNING, NECROTIC, PIERCING, POISON, 
	PSYCHIC, RADIANT, SLASHING, THUNDER
} damageType;

typedef enum {
	STR, DEX, CON, INT, CHA, WIS
} abilityType;

typedef enum {
	ABJURATION, CONJURATION, DIVINATION, EVOCATION,
	ENCHANTMENT, ILLUSION, NECROMANCY, TRANSMUTATION
} spellSchoolType;

typedef enum {
	NONE, SUBCLASS_SPELLCASTER, MINOR_SPELLCASTER, MAJOR_SPELLCASTER, CUSTOM
} spellSlotType;

typedef enum {
	BLINDED, CHARMED, DEAFENED, FRIGHTENED, GRAPPLED,
	INCAPACITATED, INVISIBLE, PARALYZED, PETRIFIED, POISONED,
	PRONE, RESTRAINED, STUNNED, UNCONSCIOUS, EXHAUSTED
} statusEffectType;

typedef enum {
	ATHLETICS, ACROBATICS, SLEIGHT_OF_HAND, STEALTH, ARCANA, HISTORY,
	INVESTIGATION, NATURE, RELIGION, ANIMAL_HANDLING, INSIGHT, MEDICINE,
	PERCEPTION, SURVIVAL, DECEPTION, INTIMIDATION, PERFORMANCE, PERSUASION
} skillType;

typedef enum {
	ACTION, BONUS_ACTION, FREE_ACTION, REACTION, AT_WILL, LONG
} actType;

typedef enum {
	MAIN_SCREEN, ABILITY_SAVE, ABILITY_CHECK, SKILL_CHECK, INITIATIVE, APPLYING_STATUS_EFFECT, COMBAT_MENU
} menuType;

typedef enum {
	SHORT_REST, LONG_REST, BEFORE_TURN, AFTER_TURN, AFTER_TAKING_STATUS_EFFECT,
	BEFORE_COMBAT, DOWNED, TAKING_DAMAGE, PRE_WEAPON_ATTACK, WEAPON_ATTACK_OPTIONS,
	PRE_WEAPON_DAMAGE, WEAPON_DAMAGE_OPTIONS, PRE_SPELLCAST, SPELLCAST_OPTIONS,
	PRE_SPELL_DAMAGE, SPELL_DAMAGE_OPTIONS
} processLocationType;

typedef enum { RTRN_INT, RTRN_BOOL, RTRN_FLOAT, RTRN_STR,
		RTRN_ABLT, RTRN_MENU, RTRN_PROCESS_TIMING, RTRN_PHASE,
		RTRN_DAMAGE, RTRN_FLAG, RTRN_EMPTY, RTRN_FAIL } returnType;
//consider adding a rtrn param fail to localize failures

typedef struct { int die[8]; } Dice;

typedef struct { int slots[10]; } SpellSlots;

typedef struct { int score[6]; } AbilityScore;

typedef struct {
	damageType type;
	Dice dice;
} Damage;

generate_dyna_headers_M(Damage);

typedef void* SpellList;

typedef struct {
	menuType menu;
	char* private_name;
	char* prereqs;
	char* message;
} Note;

generate_dyna_headers_M(Note);

Note* note_init(Note*);
int note_clear(Note*);

typedef void* (*funcType)(GPtrArray*, returnType*);

void safe_free(void* p);

alignmentType str_to_alignment(const char*);
sizeType str_to_size(const char*);
int index_to_dicenum(int);
int dicenum_to_index(int);
int xp_to_level(int);
int level_to_proficiency(int);
int ability_score_to_mod(int score);
void max_out_dice(Dice* d);
void assign_spell_slots(SpellSlots* user_data, spellSlotType tier, int class_level);

Damage* DamageInit(Damage*);

damageType str_to_damage_type(char* str);

DamageDyna* parse_damage(DamageDyna*, const char*);

char* set_string(char* destination, const char* source);

char* append_string(char* destination, const char* source);

#endif
