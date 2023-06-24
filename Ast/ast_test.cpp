#include "ast.h"
#include <cstdlib>

void testString(void){
    program p;
    
    token t1={token_type::let, "let"};
    letStatement* letStmt = new letStatement(t1);
    token t2 = {token_type::ident, "myVar"};
    letStmt->name = new identifier(t2,"myVar");
    token t3 = {token_type::ident,"anotherVar"};
    letStmt->value = new identifier(t3,"anotherVar");

    p.statements.push_back(letStmt);

    if(p.strings() != "let myVar = anotherVar;"){
        std::cerr<<"program.strings() wrong. got "<<p.strings()<<std::endl;

    exit(EXIT_FAILURE);
    }
    std::cout<<"Everything okay"<<std::endl;

}

int main(void){
    testString();
    return 0;
}

