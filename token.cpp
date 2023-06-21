#include "lexer.h"
#include <iostream>
#include <map>


std::map<std::string, token_type> keywords = {
    {"fn", token_type::function},
    {"let", token_type::let},
};

token_type token::lookupIdent(std::string ident){
    auto tok = keywords.find(ident);
    if(tok != keywords.end()){
        return tok->second;
    }
    return token_type::ident;
}
