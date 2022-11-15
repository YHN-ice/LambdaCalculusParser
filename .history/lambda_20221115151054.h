#include <stdlib.h>
#include <assert.h>
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


union Lambda
{
    char n;
    struct Function f;
    struct Application ap;
};

struct L{
    union Lambda l;
    int type;/*0-name, 1-func, 2-application*/
};

struct L *createFunction();

struct L *createApplication();

struct L *createName();

void printLambda(struct L *l);

void free_inner(struct L *l);
void clone(struct L *src, struct L *dst);

void replace(struct L *l, int from, struct L *to)
{
    #undef __DEBUG
    #ifdef __DEBUG
    printf("replaceing %c in...\n",from);
    printLambda(l);
    printf("\nto:\n");
    printLambda(to);
    printf("\n");
    #endif
    if (l->type == 1)
    {
        if(l->l.f.head == from){
            /*shadowing, do nothing*/
        }
        else{
            replace(l->l.f.body,from,to);
        }
    }
    else if(l->type == 0){
        if(l->l.n==from)
            clone(to, l);
    }
    else{
        /*don't care name collision after reduction, TODO*/
        replace(l->l.ap.func, from, to);
        replace(l->l.ap.arg, from, to);
    }
}
void strip(struct L* l){

    free_inner(l->l.ap.arg);
    free(l->l.ap.arg);

    struct L *old_ptr = l->l.ap.func;
    l->type = old_ptr->l.f.body->type;
    l->l = old_ptr->l.f.body->l;
    free_inner(old_ptr);
    free(old_ptr);
}

struct L *apply(struct L *l)
{
    assert(l->type == 2);
    struct L *sub = l->l.ap.func;
    assert(sub->type == 1);
    struct L *obj = l->l.ap.arg;
    int from = sub->l.f.head;
    struct L *to = obj;
    replace(sub->l.f.body, from, to);
    // strip(l);
    //clone(sub->l.f.body, l);
    struct L *old_ptr = l->l.ap.func;
    l->type = old_ptr->l.f.body->type;
    l->l = old_ptr->l.f.body->l;
    return l;
}