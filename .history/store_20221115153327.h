#include <stdio.h>
#include <string.h>
#include "lambda.h"

#define ID_LEN 24

/*lambda Table Entry*/
struct LaTE{
    char id[ID_LEN];
    struct L l;
};

extern struct LaTE *table;
extern int capacity;
extern int frontier;

void store_symbol(char *id, struct L *l);

int lookup_symbol(char *id);
struct L *load_symbol_copy(char *id){
    int idx = lookup_symbol(id);
    if(id<0)
        return nullptr;
    else{
        return get_copy(&(table + idx)->l);
    }
}
