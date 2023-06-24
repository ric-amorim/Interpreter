#include "lexer.h"
#include <iostream>
#include <map>


std::map<std::string, token_type> keywords = {
    {"fn", token_type::function},
    {"let", token_type::let},
    {"true", token_type::trueKey},
    {"false", token_type::falseKey},
    {"return", token_type::returnKey},
    {"if", token_type::ifKey},
    {"else", token_type::elseKey},
};

token_type token::lookupIdent(std::string ident){
    auto tok = keywords.find(ident);
    if(tok != keywords.end()){
        return tok->second;
    }
    return token_type::ident;
}

std::string tokenToString(token_type t){
    std::map<token_type,std::string> s{
        {token_type::illegal, "illegal"},
        {token_type::eof, "eof"},
        {token_type::ident, "ident"},
        {token_type::integer, "integer"},
        {token_type::assign,"="},
        {token_type::plus,"+"},
        {token_type::asterisk,"*"},
        {token_type::bang,"!"},
        {token_type::slash,"/"},
        {token_type::minus,"-"},
        {token_type::lt,"<"},
        {token_type::gt,">"},
        {token_type::eq,"="},
        {token_type::notEq,"!="},
        {token_type::comma,","},
        {token_type::semicolon,";"},
        {token_type::lparen,"("},
        {token_type::rparen,")"},
        {token_type::lbrace,"{"},
        {token_type::rbrace,"}"},
        {token_type::function,"fn"},
        {token_type::let,"let"},
        {token_type::trueKey,"true"},
        {token_type::falseKey,"false"},
        {token_type::returnKey,"return"},
        {token_type::ifKey,"if"},
        {token_type::elseKey,"else"},
    };
    auto it = s.find(t);
    if (it != s.end()) {
        return it->second;
    } else {
        return "unknown";
    }
}

