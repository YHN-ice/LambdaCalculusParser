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
    union Lambda l;
    int type;/*0-name, 1-func, 2-application*/
};

union Lambda
{
    char n;
    struct Function f;
    struct Application ap;
};

struct L *createFunction(){
    L* l = (struct L *)malloc(sizeof(struct L));
    l->type = 1;
    return l;
}

struct L *createApplication(){
    L* l = (struct L *)malloc(sizeof(struct L));
    l->type = 2;
    return l;
}

L Lambda *createLambda(){
    L* l = (struct L *)malloc(sizeof(struct L));
    return (union Lambda *)malloc(sizeof(union Lambda));
}

void printLambda(struct L *l);{
    switch (l->type)
    {
    case /* constant-expression */1:
        /* code */
        printf("func(head:%c,body:",l->l.f.head);
        printLambda(l->l.f.body);
        printf(")");
        break;
    case 2:
        printf("apply(");
        printLambda(l->l.ap.func);
        printf("->");
        printLambda(l->l.ap.arg);
        printf(")");
        break;

    default:
        printf("name:%c", l.l.n);
        break;
    }
}