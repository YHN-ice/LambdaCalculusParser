struct Function
{
    /* data */
    int head;
    union Lambda *body;
};

struct Application
{
    /* data */
    union Lambda *func;
    Lambda *arg;
};

union Lambda{
    char n;
    Function f;
    Application ap;
};
