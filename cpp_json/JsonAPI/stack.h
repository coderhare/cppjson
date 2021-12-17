//
// Created by Morisummer on 2021/12/16.
//

#ifndef CPP_JSON_STACK_H
#define CPP_JSON_STACK_H
#include "Json.h"

constexpr int PARSER_STACK_INIT_SIZE = 256;
constexpr int STRINGIFIER_INIT_SIZE = 256;
class stack_j{
public:
    const char * json; //read-only, avoid modify
    char * stack;
    size_t top, size;
    stack_j();
    void * push(size_t size);
    void parse_whitespace();
    void* pop(size_t size);
    void put_char(char c);
    void put_string(const string & s);
    void expect(char c);

};
#endif //CPP_JSON_STACK_H
