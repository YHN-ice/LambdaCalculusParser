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

union Lambda{
    char n;
    Function f;
    Application ap;
};
