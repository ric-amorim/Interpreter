#include "parser.h"
#include <vector>
#include <sstream>
#include <functional>


parser::parser(lexer l,std::vector<std::string> errors) 
     : l(l),errors(errors){
    
    nextToken();
    nextToken();

    prefixParseFns = std::map<token_type,prefixParseFn>();
    registerPrefix(token_type::ident,&parser::parseIdentifier);
    
    return;
}                                                                                                                                                                                                                                  

expression* parser::parseIdentifier(void){
    return new identifier{curToken,curToken.literal};
}

void parser::nextToken(void) noexcept{
    curToken=peekToken;
    token n;
    l.nextToken(n);
    peekToken = n;
}

bool parser::curTokenIs(token_type t) noexcept{
    return this->curToken.tokenType == t;
}

bool parser::peekTokenIs(token_type t) noexcept{
    return this->peekToken.tokenType == t;
}


bool parser:: expectPeek(token_type t) noexcept{
    if(this->peekTokenIs(t)){
        this->nextToken();
        return true;
    }
    this->peekError(t);
    return false;
}

std::vector<std::string> parser::error(void) noexcept{
    return this->errors;
}

void parser::peekError(token_type t) noexcept{
    std::string expectedToken = tokenToString(t);

    std::string actualToken = tokenToString(this->peekToken.tokenType);

    std::ostringstream oss;
    oss << "expected next token to be " << expectedToken << ", got " << actualToken << " instead";
    std::string msg = oss.str();
    this->errors.push_back(msg);
}


program* parser::parseProgram(void) noexcept{
    program* progm= new program();
    progm->statements = std::vector<statement*>();

    while(!this->curTokenIs(token_type::eof)){
       statement* stmt = parseStatement(); 
       if(stmt != nullptr)
           progm->statements.push_back(stmt);
       this->nextToken(); 
    }
    return progm;
}

statement* parser::parseStatement(void) noexcept{
    switch (this->curToken.tokenType) {
        case token_type::let:
            return this->parseLetStatement();
        case token_type::returnKey:
            return this->parseReturnStatement();
        default:
            return this->parseExpressionStatement();
    }
}

letStatement* parser::parseLetStatement(void) noexcept{
    letStatement* stmt = new letStatement(this->curToken);

    if(!this->expectPeek(token_type::ident)){
        return nullptr;
    }
    stmt->name = new identifier(this->curToken,this->curToken.literal);

    if(!this->expectPeek(token_type::assign)){
        return nullptr;
    }

    while(!this->curTokenIs(token_type::semicolon))
        this->nextToken();
    return stmt;
}

returnStatement* parser::parseReturnStatement(void) noexcept{
    returnStatement* stmt = new returnStatement(this->curToken);

    this->nextToken();

    while(!this->curTokenIs(token_type::semicolon))
        this->nextToken();
    return stmt;

}

void parser::registerPrefix(token_type t,prefixParseFn pre) noexcept{
    this->prefixParseFns[t] = pre;
}

void parser::registerInfix(token_type t,infixParseFn in) noexcept{
    this->infixParseFns[t] = in;
}

expression* parser::parseExpression(precedence p) noexcept{
    prefixParseFn prefix = this->prefixParseFns[this->curToken.tokenType];
    if(prefix == nullptr) 
        return nullptr;
    expression* leftExp = (this->*prefix)();

    return leftExp;
}

expressionStatement* parser::parseExpressionStatement(void) noexcept{
    expressionStatement* stmt = new expressionStatement(this->curToken);

    stmt->expressions = this->parseExpression(lowest);

    if(this->peekTokenIs(token_type::semicolon))
            this->nextToken();
    return stmt;

}
















