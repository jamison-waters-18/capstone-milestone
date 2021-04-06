#universal makefile
#works in any project that fits certain assumptions
#1. all .c are in the first layer of subdirectory
#2. all .c files have a unique name
CC=clang
RM=rm
FLAGS=-g -Wall `pkg-config --cflags --libs glib-2.0 mxml`
DEPFLAGS=-MMD
TARGET="a.out"
O=obj
UNUSED=unused
REMOVABLES=
							#2 lines to prepare .c files
LIST=$(wildcard */*.c)				#gets list of all .c files
LIST2=$(filter-out $(UNUSED)/%,$(LIST))	#removes files in $(UNUSED)
							#3 lines to prepare .o files
LIST3=$(subst /,/ $O/,$(LIST2))		#inserts $O directory in path
LIST4=$(filter-out %/,$(LIST3))		#cleans up after LIST3
OLIST:=$(subst .c,.o,$(LIST4))		#replaces .c with .o


all: $(TARGET)

-include $O/*.d

$(TARGET): $(OLIST)
	$(CC) -o $@ $^ $(FLAGS) 

$O/%.o: */%.c | $O				#pipe makes $O order-only
	$(CC) -c $(DEPFLAGS) -o $@ $< $(FLAGS) 

$O:
	mkdir $O

clean:
	$(RM) $(TARGET) $O/* $(REMOVABLES)

