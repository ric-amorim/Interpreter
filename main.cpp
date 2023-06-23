#include <cstdlib>
#include <iostream>
#include "Lexer/lexer.h"



int main(void){
    std::cout<<"Hello "<<getenv("USER")<<"! This is the Monkey programming language!\n";
    std::cout<<"Feel free to type in commands\n";
    repl in;
    in.start();
    exit(EXIT_SUCCESS);
}

