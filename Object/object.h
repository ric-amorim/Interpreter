#include <iostream>

enum objectType{
    integer_obj,
    boolean_oj,
    null_obj
};

class object{
public:
    virtual objectType type() const = 0;
    virtual std::string inspect() const = 0;
    virtual ~object() {}
};

class Integer : public object{
private:
    int value;
public:
    Integer(int);
    std::string inspect();
    objectType type();
};

class Boolean : public object{
private:
    bool value;
public:
    Boolean(bool);
    std::string inspect();
    objectType type();
};

class Null : public object{
public:
    std::string inspect();
    objectType type();
};


