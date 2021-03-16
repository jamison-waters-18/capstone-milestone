#ifndef GLIB_FACADE_H
#define GLIB_FACADE_H

#include <glib.h>

typedef GHashTable GStrIntHashTable;

typedef GHashTableIter GStrIntHashTableIter;

void* my_gint_new(int num);

//assumes the second parameter is TRUE, creating a 1-param version
void my_g_string_free(void* s);

GStrIntHashTable* my_g_str_int_hash_table_new();

void g_str_int_hash_table_insert(GStrIntHashTable* t, GString* s, int* i);

void g_str_int_hash_table_iter_init( GStrIntHashTableIter*, GStrIntHashTable* );

gboolean g_str_int_hash_table_iter_next( GStrIntHashTableIter*, GString**, int** );

int* g_str_int_hash_table_lookup( GStrIntHashTable*, GString* ); 

#endif	//GLIB_FACADE_H
