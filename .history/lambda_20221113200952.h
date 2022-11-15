struct Function
{
    /* data */
    int head;
    Lambda *body;
} ;

struct 
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
