//
// Created by Morisummer on 2021/12/11.
//
#include "Json.h"

namespace JSON{


/**-----------------------------------------json Implementation-----------------------------------------------**/

    /** Implementation of json constructor and operator**/
    json::json():json_ptr(make_unique<Json_value>()){}
    json::json(double n): json_ptr(make_unique<Json_value>(n)){}

    json::json(const string & s):json_ptr(make_unique<Json_value>(s)){}
    json::json(string && s):json_ptr(make_unique<Json_value>(std::move(s))){}

    json::json(const arr & a):json_ptr(make_unique<Json_value>(a)){}
    json::json(arr && a):json_ptr(make_unique<Json_value>(std::move(a))){}

    json::json(const mem & m):json_ptr(make_unique<Json_value>(m)){}
    json::json(mem && m):json_ptr(make_unique<Json_value>(std::move(m))){}

    json & json::operator=(json && rhs) noexcept {
        json_ptr = std::move(rhs.json_ptr);
        return *this;
    }

    json & json::operator=(const json & rhs) noexcept{
        if(rhs == *this) return *this; //avoid self-assignment
        puts("++");
        json tmp(rhs);
        json_ptr.swap(tmp.json_ptr);
        puts("--");
        return *this;
    }

    json::json(const json & rhs){
        switch(rhs.json_ptr->get_type()){
            case JSON_NULL: json_ptr = make_unique<Json_value>(nullptr); break;
            case JSON_STRING: json_ptr = make_unique<Json_value>(rhs.get_string()); break;
            case JSON_NUMBER: json_ptr = make_unique<Json_value>(rhs.get_number()); break;
            case JSON_ARRAY: json_ptr = make_unique<Json_value>(rhs.get_arr()); break;
            case JSON_OBJECT: json_ptr = make_unique<Json_value>(rhs.get_mem()); break;
            default: json_ptr = make_unique<Json_value>(rhs.get_bool()); break;
        }
    }

    json::json(json && rhs) noexcept {
        this->json_ptr = std::move(rhs.json_ptr);
    }

    bool json::operator==(const json & rhs) const{
        assert(this->json_ptr != nullptr && rhs.json_ptr != nullptr);
        return *this->json_ptr == *rhs.json_ptr;
    }

    bool json::operator!=(const json &rhs) const {
        return !(*this == rhs);
    }

    /** Implementation of json parser **/
    JSON_TYPE json::type() const{
        /* I think people would like more to see characters instead of numbers.. */
        return json_ptr->get_type();
    }

    const JSON_TYPE json::get_type() const{
        return json_ptr->get_type();
    }

    string json::get_type_format() const{
        JSON_TYPE  type = json_ptr->get_type();
        switch(type){
            case JSON_NULL: return "null";
            case JSON_STRING: return "string";
            case JSON_TRUE:
            case JSON_FALSE: return "boolean";
            case JSON_ARRAY: return "array";
            case JSON_OBJECT: return "object";
        }
    }


    string & json::get_string() const{
        return json_ptr->get_string();
    }
    double json::get_number() const{
        return json_ptr->get_number();
    }
    arr & json::get_arr() const{
        return json_ptr->get_arr();
    }
    mem & json::get_mem() const{
        return json_ptr->get_mem();
    }
    bool json::get_bool() const{
        return json_ptr->get_bool();
    }
    void json::set_string(string &s) {
        json_ptr->set_string(s);
    }
    void json::set_string(string && s){
        json_ptr->set_string(std::move(s));
    }
    void json::set_bool(bool b){
        json_ptr->set_bool(b);
    }
    void json::set_number(double n){
        json_ptr->set_number(n);
    }
    void json::set_arr(arr & a){
        json_ptr->set_arr(a);
    }
    void json::set_arr(arr && a){
        json_ptr->set_arr(std::move(a));
    }
    void json::set_mem(mem & m){
        json_ptr->set_mem(m);
    }
    void json::set_mem(mem && m){
        json_ptr->set_mem(std::move(m));
    }
    void json::set_null(){
        json_ptr->set_null();
    }
    void json::set_type(JSON_TYPE type){
        json_ptr->set_type(type);
    }
    void json::free(){
        json_ptr->free();
    }



/**--------------------------------------------Json_value Implementation------------------------------------**/




    /** copy operators of Json_value **/
    Json_value::Json_value(): type(JSON::JSON_NULL){}
    Json_value::Json_value(double n): var(n), type(JSON::JSON_NUMBER){}
    Json_value::Json_value(const string & s): var(s), type(JSON::JSON_STRING){puts("copy");}
    Json_value::Json_value(bool b):var(b){
        if(!b) type = JSON_FALSE;
        else type = JSON_TRUE;
    }
    Json_value::Json_value(const mem & m): var(m), type(JSON::JSON_OBJECT){}
    Json_value::Json_value(const arr & a): var(a), type(JSON::JSON_ARRAY){}
    Json_value::Json_value(const Json_value & rhs) {
        *this = rhs;
    }
    Json_value::Json_value(Json_value && rhs) noexcept{
        *this = std::move(rhs);
    }

    /** move operators of Json_value **/
    Json_value & Json_value::operator=(Json_value && rhs) noexcept{
        this->var = std::move(rhs.var);
        this->type = rhs.type;
    }
    Json_value & Json_value::operator=(const Json_value & rhs) noexcept{
        Json_value tmp(rhs);
        if(*this == tmp) return *this;
        using std::swap;  /* ref C++ primer, copy and swap, implements cctor and copy assignment */
        swap(*this, tmp);
        return * this;
    }

    bool Json_value::operator==(const Json_value &rhs) const {
        if(this->get_type() != rhs.get_type()) return false;
        switch(rhs.get_type()){
            case JSON_STRING:
                return this->get_string() == rhs.get_string();
            case JSON_NUMBER:
                return this->get_number() == rhs.get_number();
            case JSON_ARRAY: {
                const auto &l = this->get_arr(), &r = rhs.get_arr();
                if (l.size() != r.size()) return false;
                for (size_t i = 0; i < l.size(); i++) {
                    if (!(l[i] == r[i])) return false;
                }
                return true;
            }
            case JSON_OBJECT: {
                const auto & a = this->get_mem(), & b = this->get_mem();
                if (a.size() != b.size()) return false;
                for(const auto & [key, value]: a) {
                    auto it = b.find(key);
                    if (it == end(b)) return false;
                    if (!(value == it->second)) return false;
                }
                return true;
            }
            default: return true;
        }
    }
    bool Json_value::operator!=(const Json_value & rhs) const{
        return !(*this == rhs);
    }

    Json_value::Json_value(string && s) noexcept: var(std::move(s)) , type(JSON::JSON_STRING){puts("move");}
    Json_value::Json_value(mem && m) noexcept: var(m), type(JSON::JSON_OBJECT){}
    Json_value::Json_value(arr && a) noexcept: var(a), type(JSON::JSON_ARRAY){}


    string & Json_value::get_string(){
        assert(type == JSON_STRING);
        return std::get<string>(this->var);
    }

    double Json_value::get_number(){
        assert(type == JSON_NUMBER);
        return std::get<double>(this->var);
    }
    arr & Json_value::get_arr(){
        assert(type == JSON_ARRAY);
        return std::get<arr>(this->var);
    }
    mem & Json_value::get_mem(){
        assert(type == JSON_OBJECT);
        return std::get<mem>(this->var);
    }

    bool Json_value::get_bool(){
        assert(type == JSON_TRUE || type == JSON_FALSE);
        return type == JSON_TRUE;
    }

    void Json_value::set_string(string & s) {
        var = s;
        type = JSON_STRING;
    }
    void Json_value::set_string(string && s){
        var = std::move(s);
        type = JSON_STRING;
    }
    void Json_value::set_bool(bool b){
        type = b ? JSON_TRUE: JSON_FALSE;
    }
    void Json_value::set_number(double n) {
        var = n;
        type = JSON_NUMBER;
    }
    void Json_value::set_arr(arr & a){
        var = a;
        type = JSON_ARRAY;
    }
    void Json_value::set_arr(arr && a){
        var = std::move(a);
        type = JSON_ARRAY;
    }
    void Json_value::set_mem(mem & m){
        var = m;
        type = JSON_OBJECT;
    }
    void Json_value::set_mem(mem && m) {
        var = std::move(m);
        type = JSON_OBJECT;
    }
    void Json_value::set_null(){
        type = JSON_NULL;
        free();
    }
    void Json_value::set_type(JSON_TYPE type){
        this->type = type;
    }
    void Json_value::free(){ /*using ~variant to free space */
            if(type == JSON_NULL) return; /* non - initialized */
            var.~variant();              /* every time we assign a new variable we should set_null at first. */
    }
}