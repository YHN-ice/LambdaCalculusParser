#ifndef __L
#define __L
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

struct L
{
    union Lambda l;
    int type; /*0-name, 1-func, 2-application*/
};
#endif

struct L *createFunction();

struct L *createApplication();

struct L *createName();

void printLambda(struct L *l);

void free_inner(struct L *l);
void clone(struct L *src, struct L *dst);

void replace(struct L *l, int from, struct L *to);
void strip(struct L *l);

struct L *apply(struct L *l);

int appliabl
struct L *exhaustive_apply(struct L *l);

struct L *get_copy(struct L *);