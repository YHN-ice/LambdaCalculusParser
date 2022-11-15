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
    if(lookup_symbol)
    p += frontier;
    strcpy(p->id,id);
    clone(l, &(p->l));
}

int lookup_symbol(char *id){
    for (int idx = 0; idx < frontier;idx++){
        struct LaTE *cur = table + idx;
        if (strcmp(cur->id, id)==0){
            return idx;
            fprintf(stderr, "lambda expr %c existed.\n", id);
        }
    }
    return -1;
}
struct L *load_symbol(char *id){

}
