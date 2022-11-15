#include <stdio.h>
#include <string.h>
#include "lambda.h"

/*lambda Table Entry*/
struct LaTE{
    char *id;
    struct L *l;
};
extern struct LaTE *table;
extern int capacity;
extern int capacity;

void store_symbol(char *id, struct L *l){
    struct LaTE *p = table;
    for (int idx = 0; idx < capacity;idx++){
        struct LaTE *cur = p + idx;
        if (strcmp(cur->id, id)==0){
            fprintf(stderr, "lambda expr %c existed.\n", id);
            return;
        }
    }

}
