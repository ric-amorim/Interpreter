#include <exception>
#include <cctype>
#include <iostream>
#include "lexer.h"


lexer::lexer(std::string input)
    : _input(input),_position(0),_readPosition(1),_byte(input[0]){
    
    return;
}


void lexer::readChar(void) noexcept{
    if(this->_readPosition >= this->_input.length()){
        this->_byte=0;
    }else{
        this->_byte = this->_input[this->_readPosition];
    }
    this->_position = this->_readPosition;
    this->_readPosition +=1;

    return;
}

bool isLetter(char byte){
  return (('a' <= byte && byte <= 'z')||('A' <= byte && byte <= 'Z')||(byte == '_')); 
}

bool isDigit(char byte){
    return '0' <= byte && byte <= '9';
}


void lexer::nextToken(token &t) noexcept{

    this-> skipWhiteSpace();
    switch (this->_byte) {
        case '=':
            if(this->peekChar()=='='){
                char byte = this->_byte;
                this->readChar();
                std::string literal = std::string(1,byte) 
                                    + std::string(1, this->_byte);
                t.tokenType = token_type::eq;
                t.literal   = literal;
                break;
            }
            t.tokenType = token_type::assign;
            t.literal   = this->_byte;
            break;        
        case ';':
            t.tokenType = token_type::semicolon;
            t.literal   = this->_byte;
            break;
        case '(':
            t.tokenType = token_type::lparen;
            t.literal   = this->_byte;
            break;
        case ')':
            t.tokenType = token_type::rparen;
            t.literal   = this->_byte;
            break;
        case ',':
            t.tokenType = token_type::comma;
            t.literal   = this->_byte;
            break;
        case '+':
            t.tokenType = token_type::plus;
            t.literal   = this->_byte;
            break;
        case '{':
            t.tokenType = token_type::lbrace;
            t.literal   = this->_byte;
            break;
        case '}':
            t.tokenType = token_type::rbrace;
            t.literal   = this->_byte;
            break;
        case '-':
            t.tokenType = token_type::minus;
            t.literal   = this->_byte;
            break;
        case '!':
            if(this->peekChar()=='='){
                char byte = this->_byte;
                this->readChar();
                std::string literal = std::string(1,byte) 
                                    + std::string(1, this->_byte);
                t.tokenType = token_type::notEq;
                t.literal   = literal;
                break;
            }
            t.tokenType = token_type::bang;
            t.literal   = this->_byte;
            break;
        case '/':
            t.tokenType = token_type::slash;
            t.literal   = this->_byte;
            break;
        case '*':
            t.tokenType = token_type::asterisk;
            t.literal   = this->_byte;
            break;
        case '<':
            t.tokenType = token_type::lt;
            t.literal   = this->_byte;
            break;
        case '>':
            t.tokenType = token_type::gt;
            t.literal   = this->_byte;
            break;
        case 0:
            t.tokenType = token_type::eof;
            t.literal   = "\0";
            break;
        default:
            if(isLetter(this->_byte)){
                t.literal   = this -> readIdentifier();
                t.tokenType = t.lookupIdent(t.literal);
                return;
            }else if (isDigit(this->_byte)){
                t.tokenType = token_type::integer;
                t.literal   = this->readNumber();
                return;
            }else{
                t.tokenType = token_type::illegal;
                t.literal   = this -> _byte;
            }
    }
    this->readChar();
    return ;
}

void lexer::skipWhiteSpace(void) noexcept{
    while(this->_byte == ' ' || this->_byte == '\t' || 
                                this->_byte == '\n' || this->_byte == '\r')
    {
        this-> readChar();
    }

}

std::string lexer::readIdentifier(void) noexcept{
   size_t position = this-> _position; 
   while(isLetter(this->_byte)){
        this->readChar();
   }
   return this->_input.substr(position, this->_position - position);
}


std::string lexer::readNumber(void) noexcept{
    size_t position = this->_position;
    while (isDigit(this->_byte)) {
        this->readChar();
    }
    return this->_input.substr(position,this->_position - position);
}

char lexer::peekChar(void) noexcept{
    if(this->_readPosition >= this->_input.length())
        return 0;
    return this->_input[this->_readPosition];
}
