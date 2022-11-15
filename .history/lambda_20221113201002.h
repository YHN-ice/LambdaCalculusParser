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

typedef union Lambda{
    char n;
    Function f;
    Application ap;
} ;
