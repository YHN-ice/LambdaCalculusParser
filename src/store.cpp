#include <stdio.h>
#include <stdlib.h>
#include<random>

#include "store.hpp"
#include "lambda.hpp"

std::unordered_map<std::string, L> table;
std::random_device rd;
std::default_random_engine rdm(rd());

bool store_symbol(std::string id, L l){
    if(table.find(id)!=table.end()){
#ifndef PROD
        printf("symbol %s already existed, skip.\n", id.c_str());
#endif
        return false;
    } else {
        l.key = id;
        table[id] = l;
        return true;
    }
}

std::string store_random(L l, int length){
    if(l.type==VAR) {
        l.key = l.var;
        return l.key;
    }
    std::string buffer;
    do{
        buffer = "";
        for (int i = 0; i < length; i++)
        {
            char tmp = rdm() % 10;
            if (tmp < 10)
            {
                tmp += '0';
            }
            else
            {
                tmp -= 10;
                tmp += 'A';
            }
            buffer += tmp;
        }
    } while (table.find(buffer) != table.end());
    l.key = buffer;
    table[buffer] = l;
    return buffer;
}

bool has_symbol(std::string id){
    return table.find(id) != table.end();;
}

L load_symbol(std::string id){
    if(std::islower(id[0])){
        return id;
    }
    return table[id];
}

void remove_symbol(std::string id){
    if(table.find(id)!=table.end()){
        table.erase(id);
    }
}

void print_table(){
    for(auto &p: table){
        printf("%s\t:%s\n", p.first.c_str(), p.second.emit_immediate().c_str());
    }
}

int get_table_size() { return table.size(); }