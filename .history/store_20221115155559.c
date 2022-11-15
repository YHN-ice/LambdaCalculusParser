#include <stdio.h>
#include <string.h>
#include "store.h"
#include "lambda.h"

struct LaTE *table;
int capacity;
int frontier;

void init_symbol_table(){
    capacity = MAX_CAP;
    table = (struct LaTE *)malloc(sizeof(struct LaTE) * capacity);
    
}
void store_symbol(char *id, struct L *l){
    struct LaTE *p = table;
    if(lookup_symbol(id)>0)
        fprintf(stderr, "lambda expr %c existed.\n", id);
    p += frontier++;
    strcpy(p->id,id);
    clone(l, &(p->l));
}

int lookup_symbol(char *id){
    for (int idx = 0; idx < frontier;idx++){
        struct LaTE *cur = table + idx;
        if (strcmp(cur->id, id)==0){
            return idx;
        }
    }
    return -1;
}
struct L *load_symbol_copy(char *id){
    int idx = lookup_symbol(id);
    if(id<0)
        return NULL;
    else{
        return get_copy(&(table + idx)->l);
    }
}
