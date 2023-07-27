#include "environment.h"
#include <unordered_map>

environment::environment(std::unordered_map<std::string,object*> s)
            : _store(s) {
    return;
}

object* environment::get(std::string name){
    return _store[name];
}


object* environment::set(std::string name, object* val){
    _store[name] = val;
    return _store[name];
}

