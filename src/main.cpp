#include "store.hpp"
#include <string>
#include <cstdlib> 
int vm_call(std::string func, int arg);
int main(int argc, char**argv){
    char *func = argv[1];
    int arg = atoi(argv[2]);
    printf("%s(%d) RESULT: %d\n", func, arg, vm_call(std::string(func), arg));
    #ifdef USAGE
    print_call_counter();
    #endif
}
