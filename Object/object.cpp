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

std::string Boolean::inspect(){
    return std::to_string(this->value);
}

objectType Boolean::type(){
    return boolean_oj;
}

std::string Null::inspect(){
    return "null";
}

objectType Null::type(){
    return null_obj;
}
