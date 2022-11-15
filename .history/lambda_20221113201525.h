#include <stdlib.h>
struct Function
{
    /* data */
    int head;
    union Lambda *body;
};

struct Application
{
    /* data */
    union Lambda *func;
    union Lambda *arg;
};

union Lambda{
    char n;
    struct Function f;
    struct Application ap;
};

struct Function *createFunction(){
    return (struct Function *)malloc(sizeof(struct FU))
}