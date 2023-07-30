#include "object.h"
#include <unordered_map>

environment* newEnclosedEnvironment(environment* outer);

class environment{
    std::unordered_map<std::string,object*> _store;
public:
    environment* outer;
    environment(std::unordered_map<std::string,object*> _store,environment* outer);
    object* get(std::string name);
    object* set(std::string name, object* val);
};

