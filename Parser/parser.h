#include "../Ast/ast.h"
#include <vector>
#include <map>
#include <unordered_map>

enum precedence{
    lowest, 
    equals,      // ==
    lessgreater, // > or <
    sum,         // +
    product,     //*
    prefix,      // -X or !X
    call         // myfunc(X)
};


class parser final{
public:
    typedef expression* (parser::*prefixParseFn)();
    typedef expression* (parser::*infixParseFn)(expression*);

    lexer l;
    token curToken;
    token peekToken;
    std::vector<std::string> errors;
    std::map<token_type, prefixParseFn> prefixParseFns;
    std::map<token_type, infixParseFn> infixParseFns;

    parser(lexer,std::vector<std::string>);
    void nextToken(void) noexcept;
    program* parseProgram(void) noexcept;
    statement* parseStatement(void) noexcept;  
    letStatement* parseLetStatement(void) noexcept;
    bool curTokenIs(token_type) noexcept;
    bool peekTokenIs(token_type)noexcept;
    bool expectPeek(token_type) noexcept;
    std::vector<std::string> error(void) noexcept;
    void peekError(token_type) noexcept;
    returnStatement* parseReturnStatement(void) noexcept;
    void registerPrefix(token_type t,prefixParseFn pre) noexcept;
    void registerInfix(token_type t,infixParseFn in) noexcept;
    expressionStatement* parseExpressionStatement(void) noexcept;
    void noPrefixParseFnError(token_type t) noexcept;
    expression* parseExpression(int p) noexcept; 
    expression* parseIdentifier(void);
    expression* parseIntegerLiteral(void);
    expression* parsePrefixExpression(void);

    int peekPrecedence(void) noexcept;
    int curPrecedence(void) noexcept;
    expression* parseInfixExpression(expression*) noexcept;

    expression* parseBoolean(void);

    expression* parseGroupedExpression(void);

    expression* parseIfExpression(void);
    blockStatement* parseBlockStatement(void) noexcept;

    expression* parseFunctionalLiteral(void);
    std::vector<identifier*> parseFunctionParameters(void) noexcept;
};

class any {   // class to have a variable that can take various types
public:
    any() : data(nullptr) {}

    template <typename T>
    any(const T& value) : data(new holder<T>(value)) {}

    template <typename T>
    T cast() const {
        if (typeid(*data) == typeid(holder<T>)) {
            return static_cast<holder<T>*>(data)->value;
        }
        throw std::bad_cast();
    }

    template <typename T>
    bool isType() const {
        return typeid(*data) == typeid(holder<T>);
    }

private:
    class baseHolder {
    public:
        virtual ~baseHolder() {}
    };

    template <typename T>
    class holder : public baseHolder {
    public:
        holder(const T& value) : value(value) {}
        T value;
    };

    baseHolder* data;
};
