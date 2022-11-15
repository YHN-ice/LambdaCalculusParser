#ifndef STORE
#define STORE
#include "lambda.h"
#define ID_LEN 24

extern struct LaTE *table;
extern int capacity;
extern int frontier;

void store_symbol(char *id, struct L *l);

int lookup_symbol(char *id);
struct L *load_symbol_copy(char *id);
#endif