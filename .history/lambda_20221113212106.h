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

struct L{
    union Lambda l;
    int type;/*0-name, 1-func, 2-application*/
};

union Lambda
{
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

void printLambda(struct L l);{
    switch (l.type)
    {
    case /* constant-expression */1:
        /* code */
        printf("func(head:")
        break;

    default:
        printf("name:%c", l.l.n);
        break;
    }
}