#include "environment.h"
#include <unordered_map>

environment::environment(std::unordered_map<std::string,object*> s,environment* outer)
            : _store(s), outer(outer) {
    return;
}

object* environment::get(std::string name){
    auto obj = _store[name];
    if(!obj && outer != nullptr)
        obj = outer->get(name);
    return obj;
}


object* environment::set(std::string name, object* val){
    _store[name] = val;
    return _store[name];
}

environment* newEnclosedEnvironment(environment* outer){
    auto env = new environment({},nullptr);
    env->outer = outer;
    return env;
}
