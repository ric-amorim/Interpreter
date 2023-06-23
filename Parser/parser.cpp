#include "parser.h"


parser::parser(lexer l) 
     : l(l){
    
    nextToken();
    nextToken();
    
    return;
} 

void parser::nextToken(void) noexcept{
    curToken=peekToken;
    token n;
    l.nextToken(n);
    peekToken = n;
}

program* parser::parseProgram(void) noexcept{
    program* progm= new program();
    progm->statements = std::vector<statement*>();

    while(this->curToken.tokenType != token_type::eof){
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
        default:
            return nullptr;
    }
}

letStatement* parser::parseLetStatement(void) noexcept{
    letStatement* stmt = new letStatement(this->curToken);
    stmt->token1 = this->curToken;

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
    return false;
}











