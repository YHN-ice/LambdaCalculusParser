struct Function
{
    /* data */
    int head;
    Lambda *body;
};

typedef struct Application
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

typedef L union Lambda;