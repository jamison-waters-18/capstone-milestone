#ifndef GLIB_FACADE_H
#define GLIB_FACADE_H

#include <glib.h>

typedef GHashTable GStrIntHashTable;
//typedef int GStrIntHashTable;

typedef GHashTableIter GStrIntHashTableIter;

//void* my_gint_new(int num);

void my_dynamic_ptr_array_free(GPtrArray*, int (*)(void*));

GStrIntHashTable* my_g_str_int_hash_table_new();

void g_str_int_hash_table_insert(GStrIntHashTable* t, char* s, int* i);

void g_str_int_hash_table_iter_init( GStrIntHashTableIter*, GStrIntHashTable* );

gboolean g_str_int_hash_table_iter_next( GStrIntHashTableIter*, char**, int** );

int* g_str_int_hash_table_lookup( GStrIntHashTable*, char* );

int g_str_int_hash_table_remove( GStrIntHashTable*, char* );

#endif	//GLIB_FACADE_H
