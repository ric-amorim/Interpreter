#include <cstdlib>
#include <iostream>
#include <string>
#include "lexer.h"

void testNextToken(void){
    std::string input{"=+(){},;"};

    token test[9] = {
        {token_type::assign, "="},
        {token_type::plus, "+"},
        {token_type::lparen, "("},
        {token_type::rparen, ")"},
        {token_type::lbrace, "{"},
        {token_type::rbrace, "}"},
        {token_type::comma, ","},
        {token_type::semicolon, ";"},
        {token_type::eof, "\0"},
    };
    
    lexer lex(input);
    token token;

    int n=0;
    for(auto &i : test){
        lex.nextToken(token); 

        if(token.tokenType != i.tokenType){
            std::cerr<<"tests["<<n<<"]-> token_type expected: "<<i.tokenType
                <<" got: "<<token.tokenType<<std::endl;
            std::exit(EXIT_FAILURE);
        }
        else if(token.literal != i.literal){
            std::cerr<<"tests["<<n<<"]-> literal expected: "<<i.literal
                <<" got: "<<token.literal<<std::endl;
            std::exit(EXIT_FAILURE);
        }
        n++;
    }
    std::cout<< "Everything is okay"<<std::endl;
    return;
};

// TEST 2


void testNextToken2(void){
    std::string input{"let five = 5;"
                      "let ten = 10;"
                      "let add = fn(x,y) {x+y; };"
                      "let result = add(five,ten);"};

    token test[37] = {
        {token_type::let, "let"},
        {token_type::ident, "five"},
        {token_type::assign, "="},
        {token_type::integer, "5"},
        {token_type::semicolon, ";"},
        {token_type::let, "let"},
        {token_type::ident, "ten"},
        {token_type::assign, "="},
        {token_type::integer, "10"},
        {token_type::semicolon, ";"},
        {token_type::let, "let"},
        {token_type::ident, "add"},
        {token_type::assign, "="},
        {token_type::function, "fn"},
        {token_type::lparen, "("},
        {token_type::ident, "x"},
        {token_type::comma, ","},
        {token_type::ident, "y"},
        {token_type::rparen, ")"},
        {token_type::lbrace, "{"},
        {token_type::ident, "x"},
        {token_type::plus, "+"},
        {token_type::ident, "y"},
        {token_type::semicolon, ";"},
        {token_type::rbrace, "}"},
        {token_type::semicolon, ";"},
        {token_type::let, "let"},
        {token_type::ident, "result"},
        {token_type::assign, "="},
        {token_type::ident, "add"},
        {token_type::lparen, "("},
        {token_type::ident, "five"},
        {token_type::comma, ","},
        {token_type::ident, "ten"},
        {token_type::rparen, ")"},
        {token_type::semicolon, ";"},
        {token_type::eof, "\0"},
    };
    
    lexer lex(input);
    token token;

    int n=0;
    for(auto &i : test){
        lex.nextToken(token); 

        if(token.tokenType != i.tokenType){
            std::cerr<<"tests["<<n<<"]-> token_type expected: "<<i.tokenType
                <<" got: "<<token.tokenType<<std::endl;
            std::exit(EXIT_FAILURE);
        }
        else if(token.literal != i.literal){
            std::cerr<<"tests["<<n<<"]-> literal expected: "<<i.literal
                <<" got: "<<token.literal<<std::endl;
            std::exit(EXIT_FAILURE);
        }
        n++;
    }
    std::cout<< "Everything is okay"<<std::endl;
    return;
};

int main(void){
    //testNextToken();
    testNextToken2();

    exit(EXIT_SUCCESS);
}
