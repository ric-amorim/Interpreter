#include <iostream>

enum objectType{
    integer_obj,
    boolean_oj,
    null_obj
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
    std::string inspect();
    objectType type();
};

class Null : public object{
public:
    std::string inspect();
    objectType type();
};


