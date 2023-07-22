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
