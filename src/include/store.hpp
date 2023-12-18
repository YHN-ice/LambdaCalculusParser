#ifndef STORE
#define STORE
#include <unordered_map>
#include <string>
#include "lambda.hpp"

#define MAX_CAP 1000
#define EPHEMERAL_NAME_LEN 6

bool store_symbol(std::string id, L l);

std::string store_random(L l, int length = EPHEMERAL_NAME_LEN);

bool has_symbol(std::string id);

L load_symbol(std::string id);

void remove_symbol(std::string id);

void print_table();
int get_table_size();
#endif