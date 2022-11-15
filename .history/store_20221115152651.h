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

void store_symbol(char *id, struct L *l){
    struct LaTE *p = table;
    p += frontier;
    strcpy(p->id,id);
    clone(l, &(p->l));
}

int lookup_symbol(char *id){

}
struct L *load_symbol(char *id){

}
