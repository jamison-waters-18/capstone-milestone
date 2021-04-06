#ifndef DYNA_H
#define DYNA_H


#define generate_dyna_headers_M(TYPE)\
\
typedef struct __Dyna_##TYPE { TYPE* array; int size; int space; } TYPE##Dyna;\
\
TYPE##Dyna* TYPE##Dyna_init(TYPE##Dyna*); \
\
TYPE##Dyna* TYPE##Dyna_add(TYPE##Dyna*, TYPE); \
\
TYPE##Dyna* TYPE##Dyna_remove(TYPE##Dyna*, TYPE*, int (*) (TYPE*)); \
\
TYPE##Dyna* TYPE##Dyna_reserve(TYPE##Dyna*, int); \
\
int TYPE##Dyna_clear(TYPE##Dyna* dyna, int (*) (TYPE*));

#endif
