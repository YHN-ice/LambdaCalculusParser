#include <cstdlib>
#include <iostream>
#include <string>
#include "store.hpp"
int vm_call(std::string func, int arg);

void read_in_file(const char *filename);

int vm_call(std::string func, int arg)
{
    if(get_table_size()==0)read_in_file("demo.in");
    if (!has_symbol(func))
    {
#ifndef PROD
        printf("IS NULL!!!");
#endif
        read_in_file((func+std::string(".lambda")).c_str());
        if(!has_symbol(func)){
#ifndef PROD
            printf("\nSTILL NULL!!!\n");
#endif
        }
    }
    return L::call_lambda(func, arg);
}
