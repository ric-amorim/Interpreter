#include "parser.h"
#include <cstdlib>

//Note to myself: the errors give the value in hexadecimal. Change in the future

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
                 <<name<<". got="
                 <<letStmt->name<<std::endl;
        return false;
    }
    if(letStmt->name->tokenLiteral() != name){ 
        std::cerr<<"letStmt->name->tokenLiteral() not "
                 <<name<<". got= "
                 <<letStmt->name<<std::endl;
        return false;
    }
    return true;
}


void testLetStatements(void){
    std::string input{"let x = 5;"
                      "let y = 10;"
                      "let foobar = 838383;"};
    lexer lex(input);
    parser pars(lex);

    program* p = pars.parseProgram(); 
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


int main(void){
    testLetStatements();
    return 0;
}
