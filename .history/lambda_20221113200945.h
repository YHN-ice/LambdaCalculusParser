struct
{
    /* data */
    int head;
    Lambda *body;
} ;

typedef struct 
{
    /* data */
    Lambda *func;
    Lambda *arg;
}Application;

typedef union{
    char n;
    Function f;
    Application ap;
} Lambda;
