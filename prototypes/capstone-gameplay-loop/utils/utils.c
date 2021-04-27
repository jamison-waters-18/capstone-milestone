#include <stdio.h>
#include <stdarg.h>
#include "../includes/utils.h"
#include "../includes/DynaImp.h"

const char damage_type_to_string[][12] = {
	"Acid", "Bludgeoning", "Cold", "Fire", "Force",
	"Lightning", "Necrotic", "Piercing", "Poison",
	"Psychic", "Radiant", "Slashing", "Thunder" };

void safe_free(void* p) {
	if (p > (void*) 5) {
		free(p);
	}
}

sizeType str_to_size(const char* input) {
	if (!strcmp(input, "tiny")) {
		return TINY;
	}
	else if (!strcmp(input, "small")) {
		return SMALL;
	}
	else if (!strcmp(input, "medium")) {
		return MEDIUM;
	}
	else if (!strcmp(input, "large")) {
		return LARGE;
	}
	else if (!strcmp(input, "huge")) {
		return HUGE;
	}
	else if (!strcmp(input, "gargantuan")) {
		return GARGANTUAN;
	}
	printf("unknown alignment %s: setting to medium\n", input);
	return MEDIUM;
}

alignmentType str_to_alignment(const char* input) {
	if (!strcmp(input, "U")) {
		return UNALIGNED;
	}
	else if (!strcmp(input, "LG")) {
		return LAWFUL_GOOD;
	}
	else if (!strcmp(input, "NG")) {
		return NEUTRAL_GOOD;
	}
	else if (!strcmp(input, "CG")) {
		return CHAOTIC_GOOD;
	}
	else if (!strcmp(input, "LN")) {
		return LAWFUL_NEUTRAL;
	}
	else if (!strcmp(input, "TN")) {
		return TRUE_NEUTRAL;
	}
	else if (!strcmp(input, "CN")) {
		return CHAOTIC_NEUTRAL;
	}
	else if (!strcmp(input, "LE")) {
		return LAWFUL_EVIL;
	}
	else if (!strcmp(input, "NE")) {
		return NEUTRAL_EVIL;
	}
	else if (!strcmp(input, "CE")) {
		return CHAOTIC_EVIL;
	}
	printf("unknown alignment %s: setting to unaligned\n", input);
	return UNALIGNED;
}

int index_to_dicenum(int index) {
	const Dice d = { { 1, 4, 6, 8, 10, 12, 20, 100 } };
	if (index < 0 || index > 7) {
		return -1;
	}
	return d.die[index];
}

int dicenum_to_index(int die) {
	switch (die) {
	case 1:   return 0;
	case 4:   return 1;
	case 6:   return 2;
	case 8:   return 3;
	case 10:  return 4;
	case 12:  return 5;
	case 20:  return 6;
	case 100: return 7;
	default:
		printf("dicenum_to_index Warning: %i not found.\n", die);
	}
	return -1;
}

int xp_to_level(int xp) {
	if (xp < 0) return -1;
	if (xp < 300) return 1;
	if (xp < 900) return 2;
	if (xp < 2700) return 3;
	if (xp < 6500) return 4;
	if (xp < 14000) return 5;
	if (xp < 23000) return 6;
	if (xp < 34000) return 7;
	if (xp < 48000) return 8;
	if (xp < 64000) return 9;
	if (xp < 85000) return 10;
	if (xp < 100000) return 11;
	if (xp < 120000) return 12;
	if (xp < 140000) return 13;
	if (xp < 165000) return 14;
	if (xp < 195000) return 15;
	if (xp < 225000) return 16;
	if (xp < 265000) return 17;
	if (xp < 305000) return 18;
	if (xp < 355000) return 19;
	return 20;
}

int level_to_proficiency(int level) {
	if (level < 1 || level > 20) return -1;
	return (level - 1) / 4 + 2;
}

int ability_score_to_mod(int score) {
	if (score < 1 || score > 30) return -1;
	return score / 2 - 5;
}

void max_out_dice(Dice* d) {
	for(int i = 1; i < 8; i++) {
		d->die[0] += d->die[i] * index_to_dicenum(i);
		d->die[i] = 0;
	}
}

void assign_spell_slots(SpellSlots* user_data, spellSlotType tier, int class_level) {
	const int reference[21][10] = 
			{	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 3, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 4, 2, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 4, 3, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 4, 3, 2, 0, 0, 0, 0, 0, 0 },
				{ 0, 4, 3, 3, 0, 0, 0, 0, 0, 0 },
				{ 0, 4, 3, 3, 1, 0, 0, 0, 0, 0 },
				{ 0, 4, 3, 3, 2, 0, 0, 0, 0, 0 },
				{ 0, 4, 3, 3, 3, 1, 0, 0, 0, 0 },
				{ 0, 4, 3, 3, 3, 2, 0, 0, 0, 0 },
				{ 0, 4, 3, 3, 3, 2, 1, 0, 0, 0 },
				{ 0, 4, 3, 3, 3, 2, 1, 0, 0, 0 },
				{ 0, 4, 3, 3, 3, 2, 1, 1, 0, 0 },
				{ 0, 4, 3, 3, 3, 2, 1, 1, 0, 0 },
				{ 0, 4, 3, 3, 3, 2, 1, 1, 1, 0 },
				{ 0, 4, 3, 3, 3, 2, 1, 1, 1, 0 },
				{ 0, 4, 3, 3, 3, 2, 1, 1, 1, 1 },
				{ 0, 4, 3, 3, 3, 3, 1, 1, 1, 1 },
				{ 0, 4, 3, 3, 3, 3, 2, 1, 1, 1 },
				{ 0, 4, 3, 3, 3, 3, 2, 2, 1, 1 } };

	int difference = class_level - 1;

	switch (tier) {
	case NONE:
	case CUSTOM:
		return;
	case SUBCLASS_SPELLCASTER: //mild, like arcane trickster
		class_level = (class_level + 2) / 3;
		difference = (difference + 2) / 3; break;
	case MINOR_SPELLCASTER: //moderate, like ranger
		if (class_level == 1) { class_level = 0; }
		else {
			class_level = (class_level + 1) / 2;
			difference = (difference + 1) / 2;
		}
		break;
	default:
		break;
	}

	for (int i = 1; i < 10; i++) {
		user_data->slot[i] += reference[class_level][i] - reference[difference][i];
	}
}

Damage* damage_init(Damage* d) {
	const Damage base = { FORCE, { { 0, 0, 0, 0, 0, 0, 0, 0 } } };
	if (!d) { return NULL; }
	*d = base;
	return d;
}

generate_dyna_functions_M(Damage);

damageType str_to_damage_type(char* str) {
	if(!strcmp(str, "acid"))
		return ACID;
	if(!strcmp(str, "bludgeoning"))
		return BLUDGEONING;
	if(!strcmp(str, "cold"))
		return COLD;
	if(!strcmp(str, "fire"))
		return FIRE;
	if(!strcmp(str, "force"))
		return FORCE;
	if(!strcmp(str, "lightning"))
		return LIGHTNING;
	if(!strcmp(str, "necrotic"))
		return NECROTIC;
	if(!strcmp(str, "piercing"))
		return PIERCING;
	if(!strcmp(str, "poison"))
		return POISON;
	if(!strcmp(str, "psychic"))
		return PSYCHIC;
	if(!strcmp(str, "radiant"))
		return RADIANT;
	if(!strcmp(str, "slashing"))
		return SLASHING;
	if(!strcmp(str, "thunder"))
		return THUNDER;
	printf("StrToDamageType Warning: %s not found.\n", str);
	return FORCE;
}

DamageDyna* parse_damage(DamageDyna* allDamage, const char* leftToRead) {
	char theseDice[20], thisType[20], nextDice[20], nextType[20], nextDamage[50];
	int remainingTokens;
	do {
		Damage thisDamage;
		damage_init(&thisDamage);
		remainingTokens = sscanf(leftToRead, "%s %s + %s %s", theseDice, thisType, nextDice, nextType);
		if (remainingTokens > 2) {
			sprintf(nextDamage, "%s %s", nextDice, nextType);
			leftToRead = strstr(leftToRead, nextDamage);
		}
		int remainingDice;
		do {
			int thisNum = 0;
			int thisDie = 1;
			remainingDice = sscanf(theseDice, "%id%i+%s", &thisNum, &thisDie, theseDice);
			thisDamage.dice.die[dicenum_to_index(thisDie)] = thisNum;
		} while(remainingDice > 2);

		thisDamage.type = str_to_damage_type(thisType);
		DamageDyna_add(allDamage, thisDamage);
	} while(remainingTokens > 2);
	return allDamage;
}


char* dice_to_string(Dice* d) {
	char* result = NULL;
	if (!d) return result;

	for (int i = 7; i >= 0; i--) {
		if (d->die[i]) {
			int dicenum = index_to_dicenum(i);
			if (dicenum == 1) {
				if (result) {
					char* add = set_string_f(NULL, " + %d", d->die[i]);
					result = append_string(result, add);
					free(add);
				} else {
					result = set_string_f(NULL, "%d", d->die[i]);
				}
				continue;
			}
			if (result) {
				char* add = set_string_f(NULL, " + %dd%d", d->die[i], index_to_dicenum(i));
				result = append_string(result, add);
				free(add);
			} else {
				result = set_string_f(NULL, "%dd%d", d->die[i], index_to_dicenum(i));
			}
		}
	}

	if (!result) set_string(NULL, "0");
	return result;
}

char* damage_dyna_to_string(DamageDyna* damages) {
	char* result = NULL;
	if (!damages) return result;

	for (int i = 0; i < damages->size; i++) {
		char* dice = dice_to_string(&(damages->array[i].dice));
		if (!dice) {
			continue;
		}
		if (!strcmp(dice, "0")) {
			free(dice);
			continue;
		}
		if (result) {
			char* add = set_string_f(NULL, " + %s %s", dice, damage_type_to_string[damages->array[i].type]);
			result = append_string(result, add);
			free(add);
		} else {
			result = set_string_f(NULL, "%s %s", dice, damage_type_to_string[damages->array[i].type]);
		}
		free(dice);
	}

	if (!result) set_string(NULL, "0");
	return result;
}

char* set_string(char* destination, const char* source) {
	destination = realloc(destination, strlen(source) + 1);
	if (destination) {
		strcpy(destination, source);
	}
	return destination;
}

char* set_string_f(char* destination, const char* source, ...) {
	va_list args;
	va_start(args, source);
	int size = vsnprintf(NULL, 0, source, args) + 1;
	va_end(args);
	destination = realloc(destination, size);
	if (destination) {
		va_start(args, source);
		vsnprintf(destination, size, source, args);
		va_end(args);
	}
	return destination;
}

char* append_string(char* destination, const char* source) {
	destination = realloc(destination, strlen(destination) + strlen(source) + 1);
	if (destination) {
		strcat(destination, source);
	}
	return destination;
}

char* upper_string(char* input) {
	if (!input) return NULL;
	while (*input) {
		*input = (*input * (*input < 'a' || 'z' < *input)) +
		         ((*input - 32) * ('a' - 1 < *input &&
		         *input < 'z' + 1));
		input++;
	}
	return input;
}

char* lower_string(char* input) {
	if (!input) return NULL;
	while (*input) {
		*input = (*input * (*input < 'A' || 'Z' < *input)) +
		         ((*input + 32) * ('A' - 1 < *input &&
		         *input < 'Z' + 1));
		input++;
	}
	return input;
}

char* title_string(char* input) {
	if (!input) return NULL;
	char* str_begin = input;
	int title_next = 1;
	while (*input) {
		if (*input == ' ' || *input == '\r' || *input == '\n') {
			title_next = 1;
		} else if (title_next) {
			*input = (*input * (*input < 'a' || 'z' < *input)) +
				     ((*input - 32) * ('a' - 1 < *input &&
				     *input < 'z' + 1));
			title_next = 0;
		}
		input++;
	}
	return str_begin;
}

char* strip_string(char* input) {
	if (!input) return NULL;
	char* str_begin = input;
	while (*input == ' ' || *input == '\r' || *input == '\n') input++;
	char* content_begin = input;
	char* content_end = input;
	while (*input) {
		if (!(*input == ' ' || *input == '\r' || *input == '\n')) {
			content_end = input + 1;
		}
		input++;
	}
	*content_end = '\0';
	if (str_begin != content_begin) {
		while(*content_begin) {
			*(str_begin++) = *(content_begin++);
		}
	}
	return str_begin;
}

char* read_file(FILE* in) {
	char* result = NULL;
	int c;
	int string_length = 0;
	size_t memsize = 0;

	if (in == stdin) {
		getline(&result, &memsize, stdin);
		result[strlen(result) - 1] = '\0';
	}
	else {
		while( (c = fgetc(in)) != EOF ) {
			if (string_length + 1 >= memsize) {
				memsize = (string_length + 1) * 2;
				result = realloc(result, sizeof(char) * memsize);
			}
			if (!result) return NULL;
			result[string_length++] = c;
		}

		if (result) { result[string_length] = '\0'; }
	}
	return result;
}
