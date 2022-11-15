struct Function
{
    /* data */
    int head;
    Lambda *body;
} ;

struct Application
{
    /* data */
    Lambda *func;
    Lambda *arg;
};

typedef union{
    char n;
    Function f;
    Application ap;
} Lambda;
