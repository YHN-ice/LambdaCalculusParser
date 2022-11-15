typedef struct
{
    /* data */
    int head;
    Lambda *body;
} Function;

typedef struct 
{
    /* data */
    Lambda *func;
    Lambda *arg;
}Application;

typedef union Lambda{
    char n;
    Function f;
    Application ap;
};

typedef L union Lambda;