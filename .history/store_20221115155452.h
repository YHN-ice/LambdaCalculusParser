#ifndef STORE
#define STORE
#include "lambda.h"
#define ID_LEN 24

#ifndef LaTE
#define LaTE
/*lambda Table Entry*/
struct LaTE{
    char id[ID_LEN];
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