#include "string.h"
#include "lambda.h"

/*lambda Table Entry*/
struct laTE{
    char *id;
    struct L *l;
};
extern struct LaTE *table;
extern int capacity;

void store_symbol(char *id, struct L *l){
    struct LaTE *p = table;
    for (int idx = 0; idx < capacity;idx++){
        if(strcmp((p+idx)->id))
    }
}
