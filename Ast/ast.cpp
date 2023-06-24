#include <iostream>
#include <vector>
#include <sstream>
#include "ast.h"


std::string program::tokenLiteral(void) const{
    if(!statements.empty())
        return statements[0]->tokenLiteral();
    return "";
}

std::string program::strings(void) const {
    std::ostringstream out;
    
    for(auto s : this->statements){
        out<< s->strings();
    }
    return out.str();
}

letStatement::letStatement(token& tok)
           : token1(tok) {
    
            return;
}

std::string letStatement::tokenLiteral(void) const{
    return token1.literal;
}

std::string letStatement::strings(void) const{
    std::ostringstream out;

    out<< this->tokenLiteral()<<" ";
    out<< this->name->strings();
    out<<" = ";

    if(this->value != nullptr)
        out<<this->value->strings();
    out<<";";
    return out.str();
}

identifier::identifier(token& tok,std::string identifier)
           : token1(tok),value(identifier)  {
    
            return;
}
std::string identifier::tokenLiteral(void) const{
    return token1.literal;
}

std::string identifier::strings(void) const {return this->value;}

returnStatement::returnStatement(token& tok)
            : token1(tok){

            return;
}

std::string returnStatement::tokenLiteral(void) const{
    return token1.literal;
}

std::string returnStatement::strings(void) const{
    std::ostringstream out;

    out<< this->tokenLiteral()<<" ";

    if(this->returnValue != nullptr)
        out<<this->returnValue->strings();
    out<<";";
    return out.str();
}


expressionStatement::expressionStatement(token& tok)
                : token1(tok){
                    return;
}

std::string expressionStatement::tokenLiteral(void) const{
    return token1.literal;
} 

std::string expressionStatement::strings(void) const{

    if(this->expressions != nullptr)
        return this->expressions->strings();
    return " ";
}



