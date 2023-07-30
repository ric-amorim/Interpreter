#include <iostream>
#include "../Parser/parser.h"

class environment;

enum objectType{
    integer_obj,
    boolean_oj,
    null_obj,
    return_value_obj,
    error_obj,
    function_obj,
};

class object{
public:
    virtual ~object() = default;
    virtual objectType type() const = 0;
    virtual std::string inspect() const = 0;
};

class Integer : public object{
public:
    int value;
    Integer(int);
    std::string inspect() const override;
    objectType type() const override;
};

class Boolean : public object{
public:
    bool value;
    Boolean(bool);
    std::string inspect() const override;
    objectType type() const override;
};

class Null : public object{
public:
    std::string inspect() const override;
    objectType type() const override;
};

class ReturnValue : public object{
public:
    object* value;
    ReturnValue(object*);
    std::string inspect() const override;
    objectType type() const override;
};

class Error : public object{
public:
    std::string message;
    Error(std::string);
    std::string inspect() const override;
    objectType type() const override;
};

class Function : public object{
public:
    std::vector<identifier*> parameters;
    blockStatement* body;
    environment* env;
    Function(std::vector<identifier*>,blockStatement*,environment*);
    objectType type() const override;
    std::string inspect() const override;
};
