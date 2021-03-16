FLAGS=-g -Wall `pkg-config --cflags --libs gtk+-3.0`
COMPILINE=gcc $< -o $@ $(FLAGS)

static: static-layout.c
	$(COMPILINE)

dynamic: dynamic-layout.c
	$(COMPILINE) -export-dynamic
