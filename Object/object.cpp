#include "object.h"
#include <string>
#include <sstream>
#include <vector>


Integer::Integer(int val) : value(val){
    return;
}

std::string Integer::inspect() const {
    return std::to_string(this->value);
}

objectType Integer::type() const {
    return integer_obj;
}

Boolean::Boolean(bool val) : value(val){
    return;
}

std::string Boolean::inspect() const{
    if(std::to_string(this->value) == "1")
        return "true";
    return "false";
}

objectType Boolean::type() const{
    return boolean_oj;
}

std::string Null::inspect() const{
    return "null";
}

objectType Null::type() const{
    return null_obj;
}

ReturnValue::ReturnValue(object* obj) : value(obj){
    return;
}

std::string ReturnValue::inspect() const{
    return this->value->inspect();
}

objectType ReturnValue::type() const{
    return return_value_obj;
}

Error::Error(std::string m) : message(m){
    return;
}

std::string Error::inspect() const{
    return "Error: " + this->message;
}

objectType Error::type() const{
    return error_obj;
}

std::string joins(const std::vector<std::string>& s,const std::string& separator){
    std::string res;
    if(!s.empty()){
        res += s[0];
        for(size_t i= 1;i< s.size();i++)
            res += separator + s[i];
    }
    return res;
}


Function::Function(std::vector<identifier*> p, blockStatement* b, environment* env)
        : parameters(p), body(b), env(env){
    return;
}

std::string Function::inspect() const{
    std::stringstream ss;

    std::vector<std::string> params;
    for(auto p : this->parameters){
        params.push_back(p->strings());
    }
    ss<<"fn"<<"("<<joins(params,", ")<<") {\n"<<this->body->strings()<<"\n}";

    return ss.str();
}

objectType Function::type() const{
    return function_obj;
}
