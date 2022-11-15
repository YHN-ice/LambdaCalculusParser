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
    union Lambda *arg;
};

union Lambda{
    char n;
    strucrt Function f;
    Application ap;
};
