#include <stdio.h>
#include <glib.h>
#include "../includes/glib-facade.h"

int main() {
	GStrIntHashTable* tabb = my_g_str_int_hash_table_new();

	GString* strink = g_string_new("hallo ther");
	g_string_append_printf(strink, "%c matey!", 'e');
	g_str_int_hash_table_insert (tabb, strink, my_gint_new(5));

	strink = g_string_new("");
	g_string_printf(strink, "hallo %d u %d", 2, 2);
	g_str_int_hash_table_insert(tabb, strink, my_gint_new(44));

	strink = g_string_new("hallo 2 u 2");
	

	GStrIntHashTableIter itr8r;
	GString* key;
	int*  val;
	g_str_int_hash_table_iter_init( &itr8r, tabb );
	while (g_str_int_hash_table_iter_next ( &itr8r, &key, &val ) ) {
		printf("%s -> %d\n", key->str, *val);
	}
	printf("manual: %s -> %d\n", strink->str, *g_str_int_hash_table_lookup(tabb, strink));
	my_g_string_free(strink);
	g_hash_table_destroy(tabb);
	return 0;
}
