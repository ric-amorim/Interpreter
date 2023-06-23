#include <cstdlib>
#include <iostream>
#include <string>
#include "Lexer/lexer.h"


#define PROMPT ">> "


void repl::start(void) noexcept{
    for(;;){
        std::string str;

        std::cout<<PROMPT;
        std::getline(std::cin,str);
        
        lexer lex(str);
        token token;

        while(token.tokenType != token_type::eof){
            lex.nextToken(token);
            std::cout<<"{Type:"<<token.tokenType
                     <<" Literal:"<<token.literal<<" }"<<std::endl;
        }
    }
    exit(EXIT_SUCCESS);
}
