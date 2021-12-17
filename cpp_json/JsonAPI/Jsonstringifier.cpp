//
// Created by Morisummer on 2021/12/16.
//

#include "Jsonstringifier.h"

namespace Jsonstringifier{
    stringifier::stringifier(){}

    void stringifier::stringify_string(const string & s) {
        size_t size;
        char* head, *p;
        p = head = (char*) stk.push( size = len * 6 + 2); /* "\u00xx..." */
        *p++ = '\"';
        for (unsigned char ch: s) {
            switch (ch) {
                case '\"': *p++ = '\\'; *p++ = '\"'; break;
                case '\\': *p++ = '\\'; *p++ = '\\'; break;
                case '\b': *p++ = '\\'; *p++ = 'b';  break;
                case '\f': *p++ = '\\'; *p++ = 'f';  break;
                case '\n': *p++ = '\\'; *p++ = 'n';  break;
                case '\r': *p++ = '\\'; *p++ = 'r';  break;
                case '\t': *p++ = '\\'; *p++ = 't';  break;
                default:
                    if (ch < 0x20) {
                        *p++ = '\\'; *p++ = 'u'; *p++ = '0'; *p++ = '0';
                        *p++ = hex_digits[ch >> 4];
                        *p++ = hex_digits[ch & 0xF];
                    }
                    else
                        *p++ = ch;
            }
        }
        *p++ = '\"';
        stk.top -= size - (p - head);
    }

    void stringifier::stringify_value(const JSON::json & j) {
        size_t i;
        switch (j.get_type()) {
            case JSON::JSON_NULL:   stk.put_string( "null"); break;
            case JSON::JSON_FALSE:  stk.put_string("false"); break;
            case JSON::JSON_TRUE:   stk.put_string("true"); break;
            case JSON::JSON_NUMBER: stk.top -= 32 - sprintf((char*)stk.push( 32), "%.17g", j.get_number()); break;
            case JSON::JSON_STRING: stringify_string(j.get_string()); break;
            case JSON::JSON_ARRAY:
                stk.put_char('[');
                for (i = 0; i < j.get_arr().size(); i++) {
                    if (i > 0)
                        stk.put_char(',');
                    stringify_value(j.get_arr()[i]);
                }
                stk.put_char( ']');
                break;
            case JSON::JSON_OBJECT:
                stk.put_char('{');
                i = 0;
                for(const auto & [k, v]: j.get_mem()){
                    if(i > 0) stk.put_char(',');
                    stringify_string(k);
                    stk.put_char(':');
                    stringify_value(v);
                    i++;
                }
                stk.put_char('}');
                break;
            default: assert(0 && "invalid type");
        }
    }

    char* stringifier::stringify(const JSON::json & j) {
        stk.stack = (char*)malloc(stk.size = STRINGIFIER_INIT_SIZE);
        stk.top = 0;
        stringify_value(j);
        if (len)
            len = stk.top;
        stk.put_char('\0');
        return stk.stack;
    }

}

