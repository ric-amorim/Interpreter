#include "parser.h"
#include <cstdlib>
#include <iostream>
#include <vector>

void checkParserErrors(parser p){
    std::vector<std::string> errors = p.error(); 
    if(errors.size() ==0)
        return;
    std::cerr<<"parser has "<<errors.size()<<" error"<<std::endl;
    for(auto& i : errors){
        std::cerr<<"parse error: "<<i<<std::endl;
    }
    exit(EXIT_FAILURE);
}




bool testLetStatement(statement* s,std::string name){
    if(s->tokenLiteral() != "let"){
        std::cerr<<"s->tokenLiteral() not 'let'. got= "
                 <<s->tokenLiteral()<<std::endl;
        return false;
    }
    auto letStmt = dynamic_cast<letStatement*>(s);
    if(!letStmt){
        std::cerr<<"s not letStatement*. got= "
                 <<s<<std::endl;
        return false;
    }
    if(letStmt->name->value != name){
        std::cerr<<"letStmt->name->value not "
                 <<name<<". got= "
                 <<letStmt->name->value<<std::endl;
        return false;
    }
    if(letStmt->name->tokenLiteral() != name){ 
        std::cerr<<"letStmt->name->tokenLiteral() not "
                 <<name<<". got= "
                 <<letStmt->name->tokenLiteral()<<std::endl;
        return false;
    }
    return true;
}

void testLetStatements(void){
    std::string input{"let x 5;"
                      "let = 10;"
                      "let 838383;"};
    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);

    program* p = pars.parseProgram(); 
    checkParserErrors(pars);
    if(p==nullptr){
        std::cerr<<"ParseProgram() return nullptr"<<std::endl;
        std::exit(EXIT_FAILURE);
    }
    if(p->statements.size() != 3) {
        std::cerr<<"p.statements doesn't contain 3 statements. got= "
                 << p->statements.size()<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string test[3] = {
        {"x"},
        {"y"},
        {"foobar"},
    };

    int n=0;
    for(auto &i : test){
        statement* st= p->statements[n];
        n++;
        if(!testLetStatement(st,i))
            return;
    }
    std::cout<< "Everything is okay"<<std::endl;
    return;
}


void testReturnStatements(void){
    std::string input{"return 5;"
                      "return 10;"
                      "return 993322;"};
    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);

    program* p = pars.parseProgram(); 
    checkParserErrors(pars);
    if(p==nullptr){
        std::cerr<<"ParseProgram() return nullptr"<<std::endl;
        std::exit(EXIT_FAILURE);
    }
    if(p->statements.size() != 3) {
        std::cerr<<"p.statements doesn't contain 3 statements. got= "
                 << p->statements.size()<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    for(auto stmt : p->statements){
        auto returnStmt = dynamic_cast<returnStatement*>(stmt);
        if(!returnStmt){
            std::cerr<<"statements not returnStatement. got= "<<stmt<<std::endl;
                continue;
        }
        if(returnStmt->tokenLiteral() != "return"){
            std::cerr<<"returnStmt.tokenLiteral not 'return'. got "
                     <<returnStmt->tokenLiteral()<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    std::cout<< "Everything is okay"<<std::endl;
    return;
}

void testIdentifierExpression(void){
    std::string input{"foobar"};
    
    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);

    program* p = pars.parseProgram(); 
    checkParserErrors(pars);

    if(p->statements.size() !=1){
        std::cerr<<"p.statements doesn't contain 1 statement. got= "
                 << p->statements.size()<<std::endl;
        std::exit(EXIT_FAILURE);

    }

    for(auto stmt : p->statements){
        auto identStmt = dynamic_cast<expressionStatement*>(stmt);
        if(!identStmt){
            std::cerr<<"statements not expressionStatement. got= "<<stmt<<std::endl;
            exit(EXIT_FAILURE);
        }

        auto ident = dynamic_cast<identifier*>(identStmt->expressions);
        if(!ident){
            std::cerr<<"statements not identifier. got= "<<stmt<<std::endl;
            exit(EXIT_FAILURE);
        }
        if(ident->value != "foobar"){
            std::cerr<<"ident.value not 'foobar'. got "
                     <<ident->value<<std::endl;
            exit(EXIT_FAILURE);
        }
        if(ident->tokenLiteral() != "foobar"){
            std::cerr<<"ident.tokenLiteral not 'foobar'. got "
                     <<ident->tokenLiteral()<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    std::cout<< "Everything is okay"<<std::endl;
    return;


}

int main(void){
    //testLetStatements();
    //testReturnStatements();
    testIdentifierExpression();
    return 0;
}
