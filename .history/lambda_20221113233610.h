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

struct L *createFunction(){
    struct L* l = (struct L *)malloc(sizeof(struct L));
    l->type = 1;
    return l;
}

struct L *createApplication(){
    struct L* l = (struct L *)malloc(sizeof(struct L));
    l->type = 2;
    return l;
}

struct L *createName(){
    struct L* l = (struct L *)malloc(sizeof(struct L));
    l->type = 0;
    return l;
}

void printLambda(struct L *l){
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
        printf("<-");
        printLambda(l->l.ap.arg);
        printf(")");
        break;

    default:
        printf("name:%c", l->l.n);
        break;
    }
}

void free_inner(struct L *l){
    if(l->type == 0){
        return;
    }
    else if(l->type==1){
        free_inner(l->l.f.body);
        free(l->l.f.body);
    }
    else{
        free_inner(l->l.ap.func);
        free(l->l.ap.func);
        free_inner(l->l.ap.arg);
        free(l->l.ap.arg);
    }
}
void clone(struct L *src, struct L *dst){
    free_inner(dst);

    dst->type = src->type;

    if(src->type==0){
        dst->l.n = src->l.n;
    }else if(src->type==1){
        dst->l.f.head = src->l.f.head;
        dst->l.f.body = (struct L *)malloc(sizeof(struct L));
        dst->l.f.body->type = 0; /*so that following clone's clean_inner will return immediately, this is a brand new lambda, so we do not need clean it*/
        clone(src->l.f.body, dst->l.f.body);
    }else{
        dst->l.ap.func = (struct L *)malloc(sizeof(struct L));
        dst->l.ap.func->type = 0;
        clone(src->l.ap.func, dst->l.ap.func);

        dst->l.ap.arg = (struct L *)malloc(sizeof(struct L));
        dst->l.ap.arg->type = 0;
        clone(src->l.ap.arg, dst->l.ap.arg);
    }
}

void replace(struct L *l, int from, struct L *to)
{
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
        l->type = to->type;
        l->l = to->l;/* deep copy */
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
    free()
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
    strip(l);
    //clone(sub->l.f.body, l);
    return l;
}