#include "object.h"
#include <string>

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
