struct Function
{
    /* data */
    char head;
    Lambda *body;
};

struct Application
{
    /* data */
    Lambda *func;
    Lambda *arg;
};

union Lambda{
};