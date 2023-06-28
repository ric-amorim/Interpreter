#include "../Ast/ast.h"
#include <vector>
#include <map>

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
    expression* parseExpression(precedence p) noexcept; 
    expression* parseIdentifier(void);
};


