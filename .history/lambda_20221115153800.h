#include <stdlib.h>
#include <assert.h>
struct Function;

struct Application;

union Lambda;

struct L;

struct L *createFunction();

struct L *createApplication();

struct L *createName();

void printLambda(struct L *l);

void free_inner(struct L *l);
void clone(struct L *src, struct L *dst);

void replace(struct L *l, int from, struct L *to);
void strip(struct L *l);

struct L *apply(struct L *l);

struct L *get_copy(struct L *);