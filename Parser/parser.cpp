#include "parser.h"
#include "parser_tracing.h"
#include <stdexcept>
#include <vector>
#include <sstream>
#include <functional>

/* tracing
class TraceGuard {
public:
    TraceGuard(const std::string& msg) : message(msg) {
        incIdent();
        tracePrint("BEGIN " + message);
    }

    ~TraceGuard() {
        tracePrint("END " + message);
        decIdent();
    }

private:
    std::string message;
};
*/
std::unordered_map<token_type,int> precedences = {
    {token_type::eq,equals},
    {token_type::notEq,equals},
    {token_type::lt,lessgreater},
    {token_type::gt,lessgreater},
    {token_type::plus,sum},
    {token_type::minus,sum},
    {token_type::slash,product},
    {token_type::asterisk,product},
    {token_type::lparen,call}
};

parser::parser(lexer l,std::vector<std::string> errors) 
     : l(l),errors(errors){
    
    nextToken();
    nextToken();

    prefixParseFns = std::map<token_type,prefixParseFn>();
    registerPrefix(token_type::ident,&parser::parseIdentifier);
    registerPrefix(token_type::integer,&parser::parseIntegerLiteral);
    registerPrefix(token_type::bang,&parser::parsePrefixExpression);
    registerPrefix(token_type::minus,&parser::parsePrefixExpression); 
    registerPrefix(token_type::trueKey,&parser::parseBoolean);
    registerPrefix(token_type::falseKey,&parser::parseBoolean);
    registerPrefix(token_type::lparen,&parser::parseGroupedExpression);
    registerPrefix(token_type::ifKey,&parser::parseIfExpression);
    registerPrefix(token_type::function,&parser::parseFunctionalLiteral);

    infixParseFns = std::map<token_type,infixParseFn>();
    registerInfix(token_type::plus,&parser::parseInfixExpression);
    registerInfix(token_type::minus,&parser::parseInfixExpression);
    registerInfix(token_type::slash,&parser::parseInfixExpression);
    registerInfix(token_type::asterisk,&parser::parseInfixExpression);
    registerInfix(token_type::eq,&parser::parseInfixExpression);
    registerInfix(token_type::notEq,&parser::parseInfixExpression);
    registerInfix(token_type::lt,&parser::parseInfixExpression);
    registerInfix(token_type::gt,&parser::parseInfixExpression);
    registerInfix(token_type::lparen,&parser::parseCallExpression);

    return;
}                                                                                                                                                                                                                                  

expression* parser::parseCallExpression(expression* function){
    auto exp = new callExpression{this->curToken,function};
    exp->arguments = this->parseCallArguments();
    return exp;
}

std::vector<expression*> parser::parseCallArguments(void) noexcept{
    std::vector<expression*> args;
    
    if(this->peekTokenIs(token_type::rparen)){
        this->nextToken();
        return args;
    }
    this->nextToken();
    args.push_back(this->parseExpression(lowest));

    while(this->peekTokenIs(token_type::comma)){
        this->nextToken();
        this->nextToken();
        args.push_back(this->parseExpression(lowest));
    }
    if(!this->expectPeek(token_type::rparen))
        return std::vector<expression*>();
    return args;
}

expression* parser::parseFunctionalLiteral(void){
    auto lit = new functionLiteral{this->curToken};

    if(!this->expectPeek(token_type::lparen))
        return nullptr;

    lit->parameters = this->parseFunctionParameters();

    if(!this->expectPeek(token_type::lbrace))
        return nullptr;
    lit->body = this->parseBlockStatement();

    return lit;
}

std::vector<identifier*> parser::parseFunctionParameters(void) noexcept{
    std::vector<identifier*> identifiers;
    
    if(this->peekTokenIs(token_type::rparen)){
        this->nextToken();
        return identifiers;
    }
    this->nextToken();

    identifier* ident = new identifier{this->curToken,this->curToken.literal};
    identifiers.push_back(ident);

    while(this->peekTokenIs(token_type::comma)){
        this->nextToken();
        this->nextToken();
        ident = new identifier{this->curToken,this->curToken.literal};
        identifiers.push_back(ident);
    }

    if(!this->expectPeek(token_type::rparen)){
       return std::vector<identifier*>(); 
    }

    return identifiers;
}

expression* parser::parseBoolean(void){
    return new boolean{curToken,curTokenIs(token_type::trueKey)};

}

blockStatement* parser::parseBlockStatement(void) noexcept{
    auto block = new blockStatement();
    block->token1 = this->curToken;
    block->statements = std::vector<statement*>();

    this->nextToken();

    while(!this->curTokenIs(token_type::rbrace) && !this->curTokenIs(token_type::eof)){
        auto stmt = this->parseStatement();
        if(stmt != nullptr)
            block->statements.push_back(stmt);
        this->nextToken();
    }
    return block;
}

expression* parser::parseIfExpression(void){
    auto exp = new ifExpression{curToken};

    if(!this->expectPeek(token_type::lparen))
        return nullptr;
    this->nextToken();
    exp->condition = this->parseExpression(lowest);

    if(!this->expectPeek(token_type::rparen))
        return nullptr;
    if(!this->expectPeek(token_type::lbrace))
        return nullptr;

    exp->consequence = this->parseBlockStatement();

    if(this->peekTokenIs(token_type::elseKey)){
        this->nextToken();

        if(!this->expectPeek(token_type::lbrace))
            return nullptr;
        exp->alternative = this->parseBlockStatement();
    }
    return exp;
}

expression* parser::parseGroupedExpression(void){
    this->nextToken();

    auto exp = this->parseExpression(lowest);

    if(!this->expectPeek(token_type::rparen))
        return nullptr;
    return exp;
}

expression* parser::parseIdentifier(void){
    return new identifier{curToken,curToken.literal};
}

expression* parser::parseIntegerLiteral(void){
    //TraceGuard guard("parseIntegerLiteral");
    integerLiteral* lit = new integerLiteral{curToken};

    long long value;
    try{
        value = std::stoll(this->curToken.literal);
    }catch (const std::invalid_argument& e){
        std::string msg = "could not parse " + this->curToken.literal + "as integer";
        this->errors.push_back(msg);
        return nullptr;
    }catch (const std::out_of_range& e){
        std::string msg = "could not parse " + this->curToken.literal + "as integer (out of range)";
        this->errors.push_back(msg);
        return nullptr;
    }
    lit->value = value;
    return lit; 
}

expression* parser::parsePrefixExpression(void){
    //TraceGuard guard("parsePrefixExpression");
    prefixExpression* expression = new prefixExpression{curToken,curToken.literal};
    this->nextToken();    

    expression->right = this->parseExpression(prefix);

    return expression;
}

void parser::nextToken(void) noexcept{
    curToken=peekToken;
    token n;
    l.nextToken(n);
    peekToken = n;
}

bool parser::curTokenIs(token_type t) noexcept{
    return this->curToken.tokenType == t;
}

bool parser::peekTokenIs(token_type t) noexcept{
    return this->peekToken.tokenType == t;
}


bool parser:: expectPeek(token_type t) noexcept{
    if(this->peekTokenIs(t)){
        this->nextToken();
        return true;
    }
    this->peekError(t);
    return false;
}

std::vector<std::string> parser::error(void) noexcept{
    return this->errors;
}

void parser::peekError(token_type t) noexcept{
    std::string expectedToken = tokenToString(t);

    std::string actualToken = tokenToString(this->peekToken.tokenType);

    std::ostringstream oss;
    oss << "expected next token to be " << expectedToken << ", got " << actualToken << " instead";
    std::string msg = oss.str();
    this->errors.push_back(msg);
}


program* parser::parseProgram(void) noexcept{
    program* progm= new program();
    progm->statements = std::vector<statement*>();

    while(!this->curTokenIs(token_type::eof)){
       statement* stmt = parseStatement(); 
       if(stmt != nullptr)
           progm->statements.push_back(stmt);
       this->nextToken(); 
    }
    return progm;
}

statement* parser::parseStatement(void) noexcept{
    switch (this->curToken.tokenType) {
        case token_type::let:
            return this->parseLetStatement();
        case token_type::returnKey:
            return this->parseReturnStatement();
        default:
            return this->parseExpressionStatement();
    }
}

letStatement* parser::parseLetStatement(void) noexcept{
    letStatement* stmt = new letStatement(this->curToken);

    if(!this->expectPeek(token_type::ident)){
        return nullptr;
    }
    stmt->name = new identifier(this->curToken,this->curToken.literal);

    if(!this->expectPeek(token_type::assign)){
        return nullptr;
    }
    this->nextToken();
    
    stmt->value = this->parseExpression(lowest);

    if(this->peekTokenIs(token_type::semicolon))
        this->nextToken();

    return stmt;
}

returnStatement* parser::parseReturnStatement(void) noexcept{
    returnStatement* stmt = new returnStatement(this->curToken);

    this->nextToken();

    stmt->returnValue = this->parseExpression(lowest);

    if(this->peekTokenIs(token_type::semicolon))
        this->nextToken();

    return stmt;

}

void parser::registerPrefix(token_type t,prefixParseFn pre) noexcept{
    this->prefixParseFns[t] = pre;
}

void parser::registerInfix(token_type t,infixParseFn in) noexcept{
    this->infixParseFns[t] = in;
}

void parser::noPrefixParseFnError(token_type t) noexcept{
    std::ostringstream oss;
    oss << "no prefix parse func for " <<tokenToString(t)<< " found";
    std::string msg = oss.str();
    this->errors.push_back(msg);
}

expression* parser::parseExpression(int p) noexcept{
    //TraceGuard guard("parseExpression");
    prefixParseFn prefix = this->prefixParseFns[this->curToken.tokenType];
    if(prefix == nullptr){
        this->noPrefixParseFnError(this->curToken.tokenType);
        return nullptr;
    }
    expression* leftExp = (this->*prefix)();

    while(!this->peekTokenIs(token_type::semicolon) && p < this->peekPrecedence()){
        infixParseFn infix = this->infixParseFns[this->peekToken.tokenType];            
        if(infix == nullptr)
            return leftExp;
        this->nextToken();

        leftExp = (this->*infix)(leftExp);
    }

    return leftExp;
}

expressionStatement* parser::parseExpressionStatement(void) noexcept{
    //TraceGuard guard("parseExpressionStatement");
    expressionStatement* stmt = new expressionStatement(this->curToken);

    stmt->expressions = this->parseExpression(lowest);

    if(this->peekTokenIs(token_type::semicolon))
            this->nextToken();
    return stmt;

}



int parser::peekPrecedence(void) noexcept{
    auto it = precedences.find(peekToken.tokenType);
    if(it != precedences.end()) //if "it" isn't pointing to the end, means that the precedence was found
        return it->second;
    return lowest;
}

int parser::curPrecedence(void) noexcept{
    auto it = precedences.find(curToken.tokenType);
    if(it != precedences.end())
        return it->second;
    return lowest;
}

expression* parser::parseInfixExpression(expression* left) noexcept{
    //TraceGuard guard("parseInfixExpression");
    infixExpression* exp = new infixExpression{curToken,left,curToken.literal};

    int prece = this->curPrecedence();
    this->nextToken();
    exp->right = this->parseExpression(prece);

    return exp;
}

