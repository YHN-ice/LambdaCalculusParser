#ifndef __L
#define __L
#include<string>
#include<vector>
#include<unordered_map>
#define VAR 0
#define FUN 1
#define APP 2
#define KEY(x) (x).key.size() ? (x).key : store_random((x))
#ifdef USAGE
void print_call_counter();
#endif
class Impl
{
private:
    bool reduce()__attribute__((no_instrument_function));

public:
    int type;
    std::string var;
    std::vector<Impl> data;
    bool replace(std::string placeholder, Impl arg);
    static Impl i2l(int i);
    int l2i();
    static int l2i(Impl);
    Impl(int t, std::string v, std::vector<Impl> d);
    void apply();
    void dump() const;
    bool is_true();
    bool is_false();

#ifdef MUL_OPTM
    bool isMUL() const;
#endif
};

class Recursive{
    const Impl generator;
    std::vector<Impl> outer;
    int outer_apply(int);
    void collapse();

public:
    Recursive(Impl);
    int reduce(int i);
};

class L
{
public:
    L();
    // L(const L &obj);
    L(std::string);
    L(std::string head, std::string tail, bool is_func = true);
    void dump() const;
    std::string to_string() const; 
    std::string emit_immediate() const;
    static int call_lambda(std::string,int);
    bool is_ephemeral();
    L apply();
    void cleanup();
    std::string key;
    int type;
    std::string var;
    static int l2i(L l);


private:
    static L i2l(int i);
    static L from_impl(Impl);
    L recursive_apply();
    L naive_apply();
    bool is_generator();
    void replace(std::string, std::string);
    Impl to_impl();

    std::string para; 
    std::string body;

    std::string callee; 
    std::string arg;
};

#endif
