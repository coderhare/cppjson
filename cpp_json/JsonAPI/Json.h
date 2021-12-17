//
// Created by Morisummer on 2021/12/11.
//

#ifndef CPP_JSON_JSON_H
#define CPP_JSON_JSON_H
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
#include <memory>
#include <cerrno>
using std::cin, std::cout, std::endl;
using std::string, std::vector, std::unordered_map, std::variant, std::unique_ptr;
using std::make_unique, std::make_shared, std::allocator;

/* after a while of thinking, I design to use unique_ptr to avoid raw pointer points to STL. */
/* and I meet some problems, the c++17 will occur some problem of template programming while c++20 will not */
/* finally I design to develop the whole project on c++20 */


namespace JSON{

    using JSON_TYPE = enum {JSON_NULL, JSON_TRUE, JSON_FALSE, JSON_STRING, JSON_NUMBER, JSON_ARRAY, JSON_OBJECT}; /**json_value types **/

    class json;      /* json is a class with a unique_ptr points to Json_value object */
    using arr = vector<json>;
    using mem = unordered_map<string, json>;      /* json-object: using unordered_map instead of dynamic array for the O(1) visit */
    class Json_value {
    public:
        /** constructors and operators **/

        Json_value();
        Json_value(const Json_value & rhs);
        Json_value(Json_value && rhs) noexcept;
        Json_value(double n);
        Json_value(const string & s);
        Json_value(string && s) noexcept;
        Json_value(bool b);
        Json_value(const mem & m);
        Json_value(mem && m) noexcept;
        Json_value(const arr & a);
        Json_value(arr && a) noexcept;
        Json_value & operator=(const Json_value & rhs) noexcept;
        Json_value & operator=(Json_value && rhs) noexcept;
        bool operator==(const Json_value & rhs) const;
        bool operator!=(const Json_value & rhs) const;

        /** get functions**/
        JSON_TYPE get_type() const{return type;}
        const string & get_string() const;
        double get_number() const;
        const arr & get_arr() const;
        const mem & get_mem() const;
        bool get_bool() const;

        /** set functions **/
        void set_string(string & s);
        void set_string(string && s);
        void set_bool(bool b);
        void set_number(double n);
        void set_arr(arr & a);
        void set_arr(arr && a);
        void set_mem(mem & m);
        void set_mem(mem && m);
        void set_null();
        void set_type(JSON_TYPE type);
        void free();
        variant<double, string, arr, mem> var;    /* the union of different type */
        JSON_TYPE type;                 /* the type of Json_value, indeed it indicates json's type */
    };


    class json {
    public:

        json();
        /** constructors and assignment operators **/
        json(const json & rhs);
        json(json && rhs) noexcept;
        json(double n);
        json(const string & s);
        json(string && s);
        json(const arr & a);
        json(arr && a);
        json(const mem & m);
        json(mem && m);
        json & operator=(const json & rhs) noexcept;
        json & operator=(json && rhs) noexcept;



        /**Json compare functions **/
        bool operator==(const json & rhs) const;
        bool operator!=(const json & rhs) const;


        string get_type_format() const;
        const JSON_TYPE get_type() const; /* I think people would like to see
        /** get_value functions **/

        bool get_bool() const;
        const string & get_string() const;
        double get_number() const;
        const arr & get_arr() const;
        const mem & get_mem() const;

        /** set_value functions **/
        void set_string(string & s);
        void set_string(string && s);
        void set_bool(bool b);
        void set_number(double n);
        void set_arr(arr & a);
        void set_arr(arr && a);
        void set_mem(mem & m);
        void set_mem(mem && m);
        void set_null();
        void set_type(JSON_TYPE type);
        void free();
    private:
        unique_ptr<Json_value> json_ptr;


        JSON_TYPE type() const;



    };


}
#endif //CPP_JSON_JSON_H
