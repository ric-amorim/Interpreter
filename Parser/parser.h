#include "../Ast/ast.h"


class parser final{
public:
    lexer l;
    token curToken;
    token peekToken;

    parser(lexer);
    void nextToken(void) noexcept;
    program* parseProgram(void) noexcept;
    statement* parseStatement(void) noexcept;  
    letStatement* parseLetStatement(void) noexcept;
    bool curTokenIs(token_type) noexcept;
    bool peekTokenIs(token_type)noexcept;
    bool expectPeek(token_type) noexcept;
};


