//
// Created by Morisummer on 2021/12/13.
//

#ifndef CPP_JSON_JSONPARSER_H
#define CPP_JSON_JSONPARSER_H

#include "stack.h"
#include "Json.h"

namespace Jsonparser {
constexpr int PARSER_STACK_INIT_SIZE = 256;

#define STRING_ERROR(ret) do { this->stk.top = head; return ret; } while(0)


    using PARSE_RESULT = enum {  /** parse results, only PARSE_OK is ok **/
        PARSE_OK = 0,
        PARSE_EXPECT_VALUE,
        PARSE_INVALID_VALUE,
        PARSE_ROOT_NOT_SINGULAR,
        PARSE_NUMBER_TOO_BIG,
        PARSE_MISS_QUOTATION_MARK,
        PARSE_INVALID_STRING_ESCAPE,
        PARSE_INVALID_STRING_CHAR,
        PARSE_INVALID_UNICODE_HEX,
        PARSE_INVALID_UNICODE_SURROGATE,
        PARSE_MISS_COMMA_OR_SQUARE_BRACKET,
        PARSE_MISS_KEY,
        PARSE_MISS_COLON,
        PARSE_MISS_COMMA_OR_CURLY_BRACKET
    };

    class parser {
    public:
        PARSE_RESULT parse(const char * json); /* it is the central part of parse, we use it to call specialized parsers */
        parser(JSON::json & _j):j(_j) /*stk(*(new stack))*/{}
    private:
        stack_j stk;
        JSON::json & j;  //avoid arguments pass by value
    private:
        PARSE_RESULT parse_literal(const char * literal, JSON::JSON_TYPE type);

        PARSE_RESULT parse_string_raw(string & s);

        PARSE_RESULT parse_string();

        const char * parse_hex4(unsigned & u);

        void parse_utf8(unsigned u);

        PARSE_RESULT parse_number();

        PARSE_RESULT parse_arr();

        PARSE_RESULT parse_mem();

        PARSE_RESULT parse_value();

        bool is0to9(char c) const;

        bool is1to9(char c) const;


        void throw_result(PARSE_RESULT ret);

    };



}

#endif //CPP_JSON_JSONPARSER_H
