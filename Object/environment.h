#include "object.h"
#include <unordered_map>

class environment{
    std::unordered_map<std::string,object*> _store;
public:
    environment(std::unordered_map<std::string,object*> _store);
    object* get(std::string name);
    object* set(std::string name, object* val);
};

