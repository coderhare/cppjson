//
// Created by Morisummer on 2021/12/16.
//

#include "stack.h"


stack_j::stack_j():stack(nullptr), top(0), json(nullptr), size(0){}
void* stack_j::push(size_t size){
    void* ret;
    assert(size > 0);
    if (top + size >= this->size) {
        if (this->size == 0)
            this->size = PARSER_STACK_INIT_SIZE;
        while (top + size >= this->size)
            this->size += this->size >> 1;  /* c->size * 1.5 */
        stack = (char*)realloc(stack, this->size);
    }
    ret = stack + top;
    top += size;
    return ret;
}
void stack_j::parse_whitespace() {
    while(*json == ' ' || *json == '\t' || *json == '\n' || *json == '\r') json++;
}
void* stack_j::pop(size_t size){
    assert(this->top >= size);
    return this->stack + (this->top -= size);
}
void stack_j::put_char(char c){
    char * p = (char*)push(sizeof(char));
    *p = c;
}
void stack_j::put_string(const string & s){
    size_t len = s.size();
    char * p = (char*)push(sizeof(char) * len);
    for(const char & c: s) *p++ = c;
}

void stack_j::expect(char c) {
    assert(c == *json);
    json++;
}

