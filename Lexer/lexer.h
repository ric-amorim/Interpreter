#include <iostream>

enum token_type{
    illegal,
    eof,

    // Identifiers + literals
    ident,
    integer,

    //Operators
    assign,
    plus,
    minus,
    bang,
    asterisk,
    slash,
    lt,
    gt,
    eq,
    notEq,

    //Delimiters
    comma,
    semicolon,

    //{},()
    lparen,
    rparen,
    lbrace,
    rbrace,

    //Keywords
    function,
    let,
    trueKey,   
    falseKey,
    ifKey,
    elseKey,
    returnKey
};

std::string tokenToString(token_type);



struct token final{
    token_type tokenType;
    std::string literal;
    token_type lookupIdent(std::string);
};

class lexer final{
private:
    std::string _input;
    size_t      _position; // current position in input (points to current char)
    size_t      _readPosition; // current reading position in input (after current char)
    char        _byte; // current char under examination

public:
    lexer(std::string input);
    void readChar(void) noexcept;
    void nextToken(token &) noexcept;
    std::string readIdentifier(void) noexcept;
    void skipWhiteSpace(void) noexcept;
    std::string readNumber(void) noexcept;
    char peekChar(void) noexcept;
};

class repl final{
public:
    void start(void) noexcept;
};

