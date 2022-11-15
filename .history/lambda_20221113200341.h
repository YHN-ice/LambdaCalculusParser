struct Function
{
    /* data */
    int head;
    Lambda *body;
};

typedef struct 
{
    /* data */
    Lambda *func;
    Lambda *arg;
}Application;

union Lambda{
    char n;
    Function f;
    Application ap;
};

typedef L union Lambda;