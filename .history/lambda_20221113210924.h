#include <stdlib.h>
struct Function
{
    /* data */
    int head;
    union Lambda *body;
    int 
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
    return (struct Function *)malloc(sizeof(struct Function));
}

struct Application *createApplication(){
    return (struct Application *)malloc(sizeof(struct Application));
}

union Lambda *createLambda(){
    return (union Lambda *)malloc(sizeof(union Lambda));
}