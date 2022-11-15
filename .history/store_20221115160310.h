#ifndef STORE
#define STORE
#include "lambda.h"

#ifndef __LaTE
#define __LaTE
/*lambda Table Entry*/
struct LaTE{
    int id;
    struct L l;
};
#endif
#define MAX_CAP 100
extern struct LaTE *table;
extern int capacity;
extern int frontier;

void store_symbol(char *id, struct L *l);

int lookup_symbol(char *id);
struct L *load_symbol_copy(char *id);
#endif