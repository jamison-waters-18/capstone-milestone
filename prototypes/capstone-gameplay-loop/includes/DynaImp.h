#ifndef DYNAIMP_H
#define DYNAIMP_H

#include <stdlib.h>
#include <string.h>

#define DYNA_INIT_SIZE 8

#define generate_dyna_functions_M(TYPE) \
 \
TYPE##Dyna* TYPE##Dyna_init(TYPE##Dyna* dyna) { \
	const TYPE##Dyna base = {0, 0, 0}; \
	if (!dyna) { return NULL; } \
	*dyna = base; \
	return dyna; } \
\
TYPE##Dyna* TYPE##Dyna_reserve(TYPE##Dyna* dyna, int newSpace) { \
	if (!dyna) { \
		dyna = calloc(0, sizeof(TYPE##Dyna)); \
	} \
	else if (dyna->space >= newSpace) { return dyna; } \
	dyna->array = ( TYPE* ) realloc(dyna->array, sizeof(TYPE) * newSpace); \
	dyna->space = newSpace; \
	return dyna; } \
 \
TYPE##Dyna* TYPE##Dyna_add(TYPE##Dyna* dyna, TYPE newItem) { \
	if (!dyna || !dyna->space) { \
		dyna = TYPE##Dyna_reserve(dyna, DYNA_INIT_SIZE); \
	} \
	else if (dyna->size == dyna->space) { \
		dyna->space *= 2; \
		dyna->array = ( TYPE* ) realloc(dyna->array, sizeof(TYPE) * dyna->space); \
	} \
	dyna->array[(dyna->size)++] = newItem; \
	return dyna; } \
 \
 TYPE##Dyna* TYPE##Dyna_remove(TYPE##Dyna* dyna, TYPE* ptr, int (*clearElm) (TYPE*)) { \
	if (!dyna || !ptr || !dyna->size) { return NULL; } \
	else if (ptr < &(dyna->array[0]) || ptr > &(dyna->array[dyna->size - 1])) { \
		return NULL; } \
	if (clearElm) { clearElm(ptr); } \
	*ptr = dyna->array[--(dyna->size)]; \
	if (dyna->size == 0) { \
		free(dyna->array); \
		dyna->array = NULL; \
		dyna->space = 0; \
	} \
	return dyna; \
 } \
 \
int TYPE##Dyna_clear(TYPE##Dyna* dyna, int (*clearElm) (TYPE*)) { \
	if (!dyna) { return -1; } \
	if (clearElm && dyna->size > 0) { \
		for (int i = 0; i < dyna->size; i++) { \
			clearElm(&dyna->array[i]); } \
	} \
	if (dyna->space > 0) { \
		free(dyna->array); \
		dyna->array = NULL; \
		dyna->space = 0; \
	} \
	return 0; \
}	

#endif //DYNAIMP_H
