#include <iostream>
#include <vector>
#include "../Lexer/lexer.h"

struct node {
    virtual std::string tokenLiteral(void) const = 0;
    virtual std::string strings(void) const =0; 
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
    std::string strings(void) const override;
};

struct blockStatement : public node{
    token token1;
    std::vector<statement*> statements;

    void statementNode(void);
    std::string tokenLiteral(void) const override;
    std::string strings(void) const override;
};

class identifier : public expression{
public:
    token token1;
    std::string value;

    identifier(token&,std::string);
    void expressionNode(void) const override {}
    std::string tokenLiteral(void) const override;
    std::string strings(void) const override;
};

class letStatement : public statement{
public:
    token token1;
    identifier* name;
    expression* value;

    letStatement(token&);
    void statementNode(void) const override {}
    std::string tokenLiteral(void) const override;
    std::string strings(void) const override;
};

class returnStatement :public statement{
public: 
    token token1;
    expression* returnValue;

    returnStatement(token&);
    void statementNode(void) const override {}
    std::string tokenLiteral(void) const override;
    std::string strings(void) const override;
};

class expressionStatement : public statement{
public:
    token token1;
    expression* expressions;

    expressionStatement(token&);
    void statementNode(void) const override {}
    std::string tokenLiteral(void) const override;
    std::string strings(void) const override;
};


class integerLiteral : public expression{
public:
    token token1;
    int value;

    integerLiteral(token&);
    void expressionNode(void) const override {}
    std::string tokenLiteral(void) const override;
    std::string strings(void) const override;
};

class prefixExpression : public expression{
public:
    token token1;
    std::string operat;
    expression* right;

    prefixExpression(token&,std::string);
    void expressionNode(void) const override {};
    std::string tokenLiteral(void) const override;
    std::string strings(void) const override;

};

class infixExpression : public expression{
public:
    token token1;
    expression* left;
    std::string operat;
    expression* right;

    infixExpression(token&,expression*,std::string);
    void expressionNode(void) const override {};
    std::string tokenLiteral(void) const override;
    std::string strings(void) const override;

};

class boolean : public expression{
public:
    token token1;
    bool value;

    boolean(token&,bool);
    void expressionNode(void) const override {};
    std::string tokenLiteral(void) const override;
    std::string strings(void) const override;
};

class ifExpression : public expression{
public:
    token token1;
    expression* condition;
    blockStatement* consequence;
    blockStatement* alternative;

    ifExpression(token&);
    void expressionNode(void) const override {};
    std::string tokenLiteral(void) const override;
    std::string strings(void) const override;

};
