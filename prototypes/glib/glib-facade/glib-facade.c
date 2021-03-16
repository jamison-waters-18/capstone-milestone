#include "../includes/glib-facade.h"

void* my_gint_new(gint num) {
      gint* p = g_malloc(sizeof(gint));
      *p = num;
      return p;
}

//exists to get rid of some static analysis warnings
unsigned int my_g_string_hash(const void* p) {
      return g_string_hash((GString*) p);
}

//exists to get rid of some static analysis warnings
gboolean my_g_string_equal(const void* p1, const void* p2) {
      return g_string_equal((GString*) p1, (GString*) p2);
}

//assumes the second parameter is TRUE, creating a 1-param version
void my_g_string_free(void* s) {
      g_string_free((GString*)s, TRUE);
}

GStrIntHashTable* my_g_str_int_hash_table_new() {
	return g_hash_table_new_full(
                  my_g_string_hash,
                  my_g_string_equal,
                  my_g_string_free,
                  g_free );
}

void g_str_int_hash_table_insert(GStrIntHashTable* t, GString* s, int* i) {
	g_hash_table_insert(t, s, i);
}

void g_str_int_hash_table_iter_init( GStrIntHashTableIter* iter, GStrIntHashTable* t ) {
	g_hash_table_iter_init(iter, t);
}

gboolean g_str_int_hash_table_iter_next( GStrIntHashTableIter* iter, GString** s, int** i ) {
	return g_hash_table_iter_next( iter, (void*) s, (void*) i );
}

int* g_str_int_hash_table_lookup( GStrIntHashTable* t, GString* s ) {
	return (int*) g_hash_table_lookup(t, s);
}
