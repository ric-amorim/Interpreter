#include <iostream>
#include <vector>
#include "ast.h"


std::string program::tokenLiteral(void) const{
    if(!statements.empty())
        return statements[0]->tokenLiteral();
    return "";
}

letStatement::letStatement(token& tok)
           : token1(tok) {
    
            return;
}

std::string letStatement::tokenLiteral(void) const{
    return token1.literal;
}

identifier::identifier(token& tok,std::string identifier)
           : token1(tok),value(identifier)  {
    
            return;
}

std::string identifier::tokenLiteral(void) const{
    return token1.literal;
}


