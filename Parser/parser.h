#include "../Ast/ast.h"
#include <vector>


class parser final{
public:
    lexer l;
    token curToken;
    token peekToken;
    std::vector<std::string> errors;

    parser(lexer,std::vector<std::string>);
    void nextToken(void) noexcept;
    program* parseProgram(void) noexcept;
    statement* parseStatement(void) noexcept;  
    letStatement* parseLetStatement(void) noexcept;
    bool curTokenIs(token_type) noexcept;
    bool peekTokenIs(token_type)noexcept;
    bool expectPeek(token_type) noexcept;
    std::vector<std::string> error(void) noexcept;
    void peekError(token_type) noexcept;
    returnStatement* parseReturnStatement(void) noexcept;
};


