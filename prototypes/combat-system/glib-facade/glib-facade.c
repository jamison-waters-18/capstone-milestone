#include "../includes/glib-facade.h"

void* my_gint_new(gint num) {
      gint* p = g_malloc(sizeof(gint));
      *p = num;
      return p;
}

//exists to get rid of some static analysis warnings
gboolean my_string_equal(const void* p1, const void* p2) {
      return !strcmp((char*) p1, (char*) p2);
}

void my_dynamic_ptr_array_free(GPtrArray* gpa, int (*clear_func)(void*)) {
	for (int i = 0; i < gpa->len; i++) {
		if (clear_func) {
			clear_func(gpa->pdata[i]);
		}
		free(gpa->pdata[i]);
	}
	g_ptr_array_free(gpa, TRUE);
}

GStrIntHashTable* my_g_str_int_hash_table_new() {
	return g_hash_table_new_full(
                  g_str_hash,
                  my_string_equal,
                  free,
                  free );
}

void g_str_int_hash_table_insert(GStrIntHashTable* t, char* s, int* i) {
	g_hash_table_insert(t, s, i);
}

void g_str_int_hash_table_iter_init( GStrIntHashTableIter* iter, GStrIntHashTable* t ) {
	g_hash_table_iter_init(iter, t);
}

gboolean g_str_int_hash_table_iter_next( GStrIntHashTableIter* iter, char** s, int** i ) {
	return g_hash_table_iter_next( iter, (void*) s, (void*) i );
}

int* g_str_int_hash_table_lookup( GStrIntHashTable* t, char* s ) {
	return (int*) g_hash_table_lookup(t, s);
}
