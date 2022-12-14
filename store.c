#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "store.h"
#include "lambda.h"

struct LaTE *table;
int capacity;
int frontier;

void init_symbol_table(){
    capacity = MAX_CAP;
    table = (struct LaTE *)malloc(sizeof(struct LaTE) * capacity);
    frontier = 0;
}

void store_symbol(int id, struct L *l){
    struct LaTE *p = table;
    if(lookup_symbol(id)>0)
        fprintf(stderr, "lambda expr %c existed.\n", id);
    p += frontier++;
    p->id=id;
    clone(l, &(p->l));
}

int lookup_symbol(int id){
    for (int idx = 0; idx < frontier;idx++){
        struct LaTE *cur = table + idx;
        if (cur->id==id){
            return idx;
        }
    }
    return -1;
}
struct L *load_symbol_copy(int id){
    int idx = lookup_symbol(id);
    if(id<0)
        return NULL;
    else{
        return get_copy(&(table + idx)->l);
    }
}
