#include <stdlib.h>
struct Function
{
    /* data */
    int head;
    struct L *body;
};

struct Application
{
    /* data */
    struct L *func;
    struct L *arg;
};

struct L{
     l;
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
        printf("func(head:%c,body:",l.l.f.head);
        printLambda()
        break;

    default:
        printf("name:%c", l.l.n);
        break;
    }
}