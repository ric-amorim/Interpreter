#include <iostream>
#include <vector>
#include "lexer.h"

struct node {
    virtual std::string tokenLiteral(void) const = 0;
    virtual ~node() {}
};

struct statement : public node{
    virtual void statementNode(void) const = 0;
    virtual ~statement() {};
};

struct expression : public node{
    virtual void expressionNode(void) const = 0;
    virtual ~expression() {};
};

struct program : public node{
    std::vector<statement*> statements;
    std::string tokenLiteral(void) const override;
};

class identifier : public expression{
public:
    token token1;
    std::string value;

    identifier(token&,std::string);
    void expressionNode(void) const override {}
    std::string tokenLiteral(void) const override;
};

class letStatement : public statement{
public:
    token token1;
    identifier* name;
    expression* value;

    letStatement(token&);
    void statementNode(void) const override {}
    std::string tokenLiteral(void) const override;
};




