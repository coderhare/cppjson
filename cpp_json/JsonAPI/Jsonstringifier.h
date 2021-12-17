//
// Created by Morisummer on 2021/12/13.
//

#ifndef CPP_JSON_JSONSTRINGIFIER_H
#define CPP_JSON_JSONSTRINGIFIER_H

#include "stack.h"
#include "Json.h"

namespace Jsonstringifier{

    static constexpr char hex_digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    class stringifier{
    public:
        stringifier();
        void stringify_string(const string & s);
        void stringify_value(const JSON::json & j);
        char* stringify(const JSON::json & j);
    private:
        stack_j stk;
        size_t len; /* the length of the json-objects */
    };
}

#endif //CPP_JSON_JSONSTRINGIFIER_H
