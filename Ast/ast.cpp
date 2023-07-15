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

integerLiteral::integerLiteral(token& t) 
            : token1(t){
    
    return;
}

std::string integerLiteral::tokenLiteral(void) const {
    return token1.literal;
}

std::string integerLiteral::strings(void) const {
    return token1.literal;
}

prefixExpression::prefixExpression(token& t,std::string operat)
             : token1(t), operat(operat){

    return;
}

std::string prefixExpression::tokenLiteral(void) const{
    return token1.literal;
}

std::string prefixExpression::strings(void) const{
    std::stringstream out;
    out << "("<<this->operat<<this->right->strings()<<")";
    return out.str();
}

infixExpression::infixExpression(token& t,expression* l,std::string operat)
            : token1(t), left(l),operat(operat){

            return;
}

std::string infixExpression::tokenLiteral(void) const{
    return token1.literal;
}

std::string infixExpression::strings(void) const{
    std::stringstream out;
    out<<"("<<this->left->strings()<<" "<<this->operat<<" "
        << this->right->strings()<<")";
    return out.str();
}


boolean::boolean(token& t,bool b)
      : token1(t), value(b){

        return;
}

std::string boolean::tokenLiteral(void) const{
    return token1.literal;
}

std::string boolean::strings(void) const{
    return token1.literal;
}

ifExpression::ifExpression(token& t)
        : token1(t){
    return;
}

std::string ifExpression::tokenLiteral(void) const{
    return token1.literal;
}

std::string ifExpression::strings(void) const{
    std::stringstream out;
    out<<"if"<<condition->strings()<<" "<<consequence->strings();

    if(alternative !=nullptr){
        out<<"else "<<alternative->strings();
    }
    return out.str();
}

std::string blockStatement::tokenLiteral(void) const{
    return token1.literal;
}

std::string blockStatement::strings(void) const {
    std::stringstream out;
    
    for(const auto& s : statements)
        out<<s->strings();
    return out.str();
}

