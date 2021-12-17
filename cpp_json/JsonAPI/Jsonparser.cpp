//
// Created by Morisummer on 2021/12/13.
//

#include "Jsonparser.h"
#include <exception>

namespace Jsonparser{

    PARSE_RESULT parser::parse(const char * json){
        PARSE_RESULT ret;
        this->stk.json = json;
        stk.stack = nullptr;
        stk.size = stk.top = 0;
        stk.parse_whitespace();
        if((ret = parse_value()) == PARSE_OK){
            stk.parse_whitespace();
            if(*this->stk.json != '\0') {
                ret = PARSE_ROOT_NOT_SINGULAR;
            }
        }
        throw_result(ret);
        free(this->stk.stack);
        return ret;
    }

    PARSE_RESULT  parser::parse_value(){
        switch(*stk.json){
            case 't': return parse_literal("true", JSON::JSON_TRUE);
            case 'f': return parse_literal("false", JSON::JSON_FALSE);
            case 'n': return parse_literal("null", JSON::JSON_NULL);
            case '\"': return parse_string();
            case '[': return parse_arr();
            case '{': return parse_mem();
            case '\0': return PARSE_EXPECT_VALUE;
            default: return parse_number();
        }
    }
    PARSE_RESULT parser::parse_literal(const char * literal, JSON::JSON_TYPE type) {

        stk.expect(literal[0]);

        size_t i = 0;
        for(; literal[i + 1] != '\0'; i++) {
            if (stk.json[i] != literal[i + 1])
                return PARSE_INVALID_VALUE;
        }
        stk.json += i;
        j.set_type(type);
        return PARSE_OK;
    }

    const char * parser::parse_hex4(unsigned & u){
        u = 0;
        for(size_t i = 0; i < 4; i++){
            char ch = *stk.json++;
            u <<= 4;
            if(is0to9(ch)) u |= ch - '0';
            else if(ch >= 'A' && ch <= 'F') u |= ch - ('A' - 10);
            else if(ch >= 'a' && ch <= 'f') u |= ch - ('a' - 10);
            return nullptr;
        }
        return stk.json;
    }

    void parser::parse_utf8(unsigned int u) {
        if(u <= 0x7F)
            stk.put_char(char(u & 0xFF));
        else if(u <= 0x7FF){
            stk.put_char(char(0xC0 | ((u >> 6) & 0xFF)));
            stk.put_char(char(0x80 | (u & 0x3F)));
        }
        else if(u <= 0xFFFF){
            stk.put_char(char(0xE0 | (u >> 12) & 0xFF));
            stk.put_char(char(0x80 | ((u >> 6) & 0x3F)));
            stk.put_char(char(0x90 | (u & 0x3F)));
        }
        else{
            assert(u <= 0x10FFFF);
            stk.put_char(char(0xF0 | ((u >> 18) & 0xFF)));
            stk.put_char(char(0x80 | ((u >> 12) & 0x3F)));
            stk.put_char(char(0x80 | ((u >> 6) & 0x3F)));
            stk.put_char(char(0x80 | (u & 0x3F)));
        }
    }

    PARSE_RESULT parser::parse_string_raw(char **str, size_t &len) {
        size_t head = this->stk.top;
        unsigned u, u2;
        stk.expect('\"');
        while(true){
            char ch = *stk.json++;
            switch(ch){
                case '\"':{
                    len = this->stk.top - head;
                    *str = (char*)stk.pop(len);
                    return PARSE_OK;
                }
                case '\\':
                    switch(*stk.json++){
                        case '\"': stk.put_char('\"'); break;
                        case '\\': stk.put_char('\\'); break;
                        case '/': stk.put_char('/'); break;
                        case 'b': stk.put_char('\b'); break;
                        case 'f': stk.put_char('\f'); break;
                        case 'n': stk.put_char('\n'); break;
                        case 'r': stk.put_char('\r'); break;
                        case 't': stk.put_char('\t'); break;
                        case 'u':{
                            //utf-8
                            if(!(stk.json = parse_hex4(u)))
                                STRING_ERROR(PARSE_INVALID_UNICODE_HEX);
                            if(u >= 0xD800 && u <= 0xDBFF){ /* surrogate pair */
                                if(*stk.json++ != '\\')
                                    STRING_ERROR(PARSE_INVALID_UNICODE_SURROGATE);
                                if(*this->stk.json++ != 'u')
                                    STRING_ERROR(PARSE_INVALID_UNICODE_SURROGATE);
                                if(!(this->stk.json = parse_hex4(u2)))
                                    STRING_ERROR(PARSE_INVALID_UNICODE_HEX);
                                if(u2 < 0xDC00 || u2 > 0xDFFF)
                                    STRING_ERROR(PARSE_INVALID_UNICODE_SURROGATE);
                                u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                            }
                            parse_utf8(u);
                            break;
                        }
                    default:
                        STRING_ERROR(PARSE_INVALID_STRING_ESCAPE);
                    }
                    break;
                case '\0':
                    STRING_ERROR(PARSE_MISS_QUOTATION_MARK);
                default:
                    if((unsigned char) ch < 0x20)
                        STRING_ERROR(PARSE_INVALID_STRING_CHAR);
                    stk.put_char(ch);
            }
        }
    }

    PARSE_RESULT parser::parse_string(){
        PARSE_RESULT ret;
        char * s;
        size_t len;
        if((ret = parse_string_raw(&s, len)) == PARSE_OK) {
            j.set_string(s);
        }
        return ret;
    }
    PARSE_RESULT parser::parse_number(){
        if(*this->stk.json == '-') this->stk.json++;
        if(*this->stk.json == '0') this->stk.json++;
        else{
            if(!is1to9(*stk.json)) return PARSE_INVALID_VALUE;
            for(stk.json++; is0to9(*stk.json); stk.json++);
        }
        if(*stk.json == '.'){
            stk.json++;
            if(!is0to9(*stk.json)) return PARSE_INVALID_VALUE;
            for(stk.json++; is0to9(*stk.json); stk.json++);
        }
        if(*stk.json == 'e' || *stk.json == 'E'){
            stk.json++;
            if(*stk.json == '+' || *stk.json == '-') stk.json++;
            if(!is0to9(*stk.json)) return PARSE_INVALID_VALUE;
            for(stk.json++; is0to9(*stk.json); stk.json++);
        }
        errno = 0;
        j.set_null();
        j.set_number(strtod(this->stk.json, nullptr));
        if(errno == ERANGE && (j.get_number() == HUGE_VAL || j.get_number() == -HUGE_VAL))
            return PARSE_NUMBER_TOO_BIG;
        return PARSE_OK;
    }

    PARSE_RESULT parser::parse_arr(){
        PARSE_RESULT ret;
        stk.expect('[');
        //considering uses vector, we don't reuse stack here.
        JSON::arr a;
        stk.parse_whitespace();
        if(*stk.json == ']'){
            stk.json++;
            j.set_null();
            j.set_arr({});
            return PARSE_OK;
        }
        while(true){
            if((ret = parse_value()) != PARSE_OK)
                return ret;
            a.push_back(j); /* copy and build json object */
            stk.parse_whitespace();
            if(*stk.json == ',') {
                stk.json++;
                stk.parse_whitespace();
            }
            else if(*stk.json == ']'){
                stk.json++;
                j.set_arr(a);
                return PARSE_OK;
            }
            else
                return PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
        }
    }

    PARSE_RESULT parser::parse_mem(){
        JSON::mem m;
        PARSE_RESULT ret;
        stk.expect('{');
        stk.parse_whitespace();
        if(*stk.json == '}'){
            stk.json++;
            j.set_mem(m);
            return PARSE_OK;
        }
        while(true){
            if(*stk.json != '\"') return PARSE_MISS_KEY;
            char * str;
            size_t len;
            if((ret = parse_string_raw(&str, len)) != PARSE_OK)
                return ret;
            //we parse key and value and then we make_pair
            stk.parse_whitespace();
            string key(str, len);
            if(*stk.json != ':') return PARSE_MISS_COLON;
            stk.json++;
            stk.parse_whitespace();
            if((ret = parse_value()) != PARSE_OK) return ret;
            m[key] = j;
            j.set_null();
            stk.parse_whitespace();
            if(*stk.json == ','){
                stk.json++;
                stk.parse_whitespace();
            }
            else if(*stk.json == '}'){
                stk.json++;
                j.set_null();
                j.set_mem(m);
                return PARSE_OK;
            }
            else return PARSE_MISS_COMMA_OR_CURLY_BRACKET;
        }
    }


    bool parser::is0to9(char c) const {return c >= '0' && c <= '9';}

    bool parser::is1to9(char c) const{return c >= '1' && c <= '9';}



    void parser::throw_result(PARSE_RESULT ret){
        switch(ret){
            case PARSE_OK: puts("parse successful!"); break;
            case PARSE_EXPECT_VALUE:{
                j.set_null();   /* we should free the space while error occurs. */
                puts("expect value while parsing, please check your input!");
                break;
            }
            case PARSE_INVALID_VALUE:{
                j.set_null();
                puts("invalid value  while parsing, please check your input!");
                break;
            }
            case PARSE_ROOT_NOT_SINGULAR:{
                j.set_null();
                puts("expect value  while parsing, please check your input!");
                break;
            }
            case PARSE_NUMBER_TOO_BIG:{
                j.set_null();
                puts("too big number  while parsing number, please check your input!");
                break;
            }
            case PARSE_MISS_QUOTATION_MARK:{
                j.set_null();
                puts("string missing quotation mark  while parsing, please check your input!");
                break;
            }
            case PARSE_INVALID_STRING_ESCAPE:{
                j.set_null();
                puts("invalid string escape while parsing string, please check your input!");
                break;
            }
            case PARSE_INVALID_STRING_CHAR:{
                j.set_null();
                puts("invalid char while parsing string, please check your input!");
                break;
            }
            case PARSE_INVALID_UNICODE_HEX:{
                j.set_null();
                puts("expect value  while parsing string, please check your input!");
                break;
            }
            case PARSE_INVALID_UNICODE_SURROGATE:{
                j.set_null();
                puts("invalid unicode surrogate  while parsing string, please check your input!");
                break;
            }
            case PARSE_MISS_COMMA_OR_SQUARE_BRACKET:{
                j.set_null();
                puts("missing comma while parsing array, please check your input!");
                break;
            }
            case PARSE_MISS_KEY:{
                j.set_null();
                puts("missing key while parsing object, please check your input!");
                break;
            }
            case PARSE_MISS_COLON:{
                j.set_null();
                puts("missing colon while parsing, please check your input!");
                break;
            }
            case PARSE_MISS_COMMA_OR_CURLY_BRACKET:{
                j.set_null();
                puts("missing comma or curly bracket occurs while parsing, please check your input!");
                break;
            }
        }
    }

}