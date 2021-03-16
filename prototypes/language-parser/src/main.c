#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "../includes/parser.h"

int main(int argc, char* argv[]) {
	FILE* in = fopen("data/sequence", "r");
	char sequence[500];
	fgets(sequence, 500, in);
	fclose(in);
	parse_sequence(sequence);
	return 0;
}
