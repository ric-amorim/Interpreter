#include "parser.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <variant>

void checkParserErrors(parser p){
    std::vector<std::string> errors = p.error(); 
    if(errors.size() ==0)
        return;
    std::cerr<<"parser has "<<errors.size()<<" error"<<std::endl;
    for(auto& i : errors){
        std::cerr<<"parse error: "<<i<<std::endl;
    }
    exit(EXIT_FAILURE);
}




bool testLetStatement(statement* s,std::string name){
    if(s->tokenLiteral() != "let"){
        std::cerr<<"s->tokenLiteral() not 'let'. got= "
                 <<s->tokenLiteral()<<std::endl;
        return false;
    }
    auto letStmt = dynamic_cast<letStatement*>(s);
    if(!letStmt){
        std::cerr<<"s not letStatement*. got= "
                 <<s<<std::endl;
        return false;
    }
    if(letStmt->name->value != name){
        std::cerr<<"letStmt->name->value not "
                 <<name<<". got= "
                 <<letStmt->name->value<<std::endl;
        return false;
    }
    if(letStmt->name->tokenLiteral() != name){ 
        std::cerr<<"letStmt->name->tokenLiteral() not "
                 <<name<<". got= "
                 <<letStmt->name->tokenLiteral()<<std::endl;
        return false;
    }
    return true;
}

void testLetStatements(void){
    std::string input{"let x 5;"
                      "let = 10;"
                      "let 838383;"};
    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);

    program* p = pars.parseProgram(); 
    checkParserErrors(pars);
    if(p==nullptr){
        std::cerr<<"ParseProgram() return nullptr"<<std::endl;
        std::exit(EXIT_FAILURE);
    }
    if(p->statements.size() != 3) {
        std::cerr<<"p.statements doesn't contain 3 statements. got= "
                 << p->statements.size()<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string test[3] = {
        {"x"},
        {"y"},
        {"foobar"},
    };

    int n=0;
    for(auto &i : test){
        statement* st= p->statements[n];
        n++;
        if(!testLetStatement(st,i))
            return;
    }
    std::cout<< "Everything is okay"<<std::endl;
    return;
}


void testReturnStatements(void){
    std::string input{"return 5;"
                      "return 10;"
                      "return 993322;"};
    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);

    program* p = pars.parseProgram(); 
    checkParserErrors(pars);
    if(p==nullptr){
        std::cerr<<"ParseProgram() return nullptr"<<std::endl;
        std::exit(EXIT_FAILURE);
    }
    if(p->statements.size() != 3) {
        std::cerr<<"p.statements doesn't contain 3 statements. got= "
                 << p->statements.size()<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    for(auto stmt : p->statements){
        auto returnStmt = dynamic_cast<returnStatement*>(stmt);
        if(!returnStmt){
            std::cerr<<"statements not returnStatement. got= "<<stmt<<std::endl;
                continue;
        }
        if(returnStmt->tokenLiteral() != "return"){
            std::cerr<<"returnStmt.tokenLiteral not 'return'. got "
                     <<returnStmt->tokenLiteral()<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    std::cout<< "Everything is okay"<<std::endl;
    return;
}

void testIdentifierExpression(void){
    std::string input{"foobar"};
    
    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);

    program* p = pars.parseProgram(); 
    checkParserErrors(pars);

    if(p->statements.size() !=1){
        std::cerr<<"p.statements doesn't contain 1 statement. got= "
                 << p->statements.size()<<std::endl;
        std::exit(EXIT_FAILURE);

    }

    for(auto stmt : p->statements){
        auto identStmt = dynamic_cast<expressionStatement*>(stmt);
        if(!identStmt){
            std::cerr<<"statements not expressionStatement. got= "<<stmt<<std::endl;
            exit(EXIT_FAILURE);
        }

        auto ident = dynamic_cast<identifier*>(identStmt->expressions);
        if(!ident){
            std::cerr<<"statements not identifier. got= "<<stmt<<std::endl;
            exit(EXIT_FAILURE);
        }
        if(ident->value != "foobar"){
            std::cerr<<"ident.value not 'foobar'. got "
                     <<ident->value<<std::endl;
            exit(EXIT_FAILURE);
        }
        if(ident->tokenLiteral() != "foobar"){
            std::cerr<<"ident.tokenLiteral not 'foobar'. got "
                     <<ident->tokenLiteral()<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    std::cout<< "Everything is okay"<<std::endl;
    return;


}

void testIntegerLiteralExpression(void){
    std::string input{"5;"};
    
    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);

    program* p = pars.parseProgram(); 
    checkParserErrors(pars);

    if(p->statements.size() !=1){
        std::cerr<<"p.statements doesn't contain 1 statement. got= "
                 << p->statements.size()<<std::endl;
        std::exit(EXIT_FAILURE);

    }

    for(auto stmt : p->statements){
        auto intStmt = dynamic_cast<expressionStatement*>(stmt);
        if(!intStmt){
            std::cerr<<"statements not expressionStatement. got= "<<intStmt<<std::endl;
            exit(EXIT_FAILURE);
        }

        auto intLiteral = dynamic_cast<integerLiteral*>(intStmt->expressions);
        if(!intLiteral){
            std::cerr<<"exp not integerLiteral. got= "<<intStmt->expressions<<std::endl;
            exit(EXIT_FAILURE);
        }
        if(intLiteral->value != 5){
            std::cerr<<"ident.value not '5'. got "
                     <<intLiteral->value<<std::endl;
            exit(EXIT_FAILURE);
        }
        if(intLiteral->tokenLiteral() != "5"){
            std::cerr<<"ident.tokenLiteral not '5'. got "
                     <<intLiteral->tokenLiteral()<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    std::cout<< "Everything is okay"<<std::endl;
    return;


}

bool testIntegerLiteral(expression* il, int value) {
    auto intLiteral = dynamic_cast<integerLiteral*>(il);
    if(!intLiteral){
        std::cerr<<"exp not integerLiteral. got= "<<il<<std::endl;
        exit(EXIT_FAILURE);
    }
    if(intLiteral->value != value){
        std::cerr<<"ident.value not '"<<value<<"'. got "
                 <<intLiteral->value<<std::endl;
        exit(EXIT_FAILURE);
    }
    if(intLiteral->tokenLiteral() != std::to_string(value)){
        std::cerr<<"ident.tokenLiteral not '"<<value<<"'. got "
                 <<intLiteral->tokenLiteral()<<std::endl;
        exit(EXIT_FAILURE);
    }
    return true;

}

void testParsingPrefixExpression(void){
    struct prefixTests{
        std::string input;
        std::string operat;
        int integerValue;
    };

    prefixTests input[4] = {{"!5;","!",5},
                           {"-15;","-",15},
                           {"!true;","!",true},
                           {"!false;","!",false}
    };
    
    for(int i=0;i<4;i++){
        lexer lex(input[i].input);
        std::vector<std::string> v;
        parser pars(lex,v);

        program* p = pars.parseProgram(); 
        checkParserErrors(pars);

        if(p->statements.size() !=1){
            std::cerr<<"p.statements doesn't contain 1 statement. got= "
                     << p->statements.size()<<std::endl;
            std::exit(EXIT_FAILURE);

        }

        for(auto stmt : p->statements){
            auto intStmt = dynamic_cast<expressionStatement*>(stmt);
            if(!intStmt){
                std::cerr<<"statements not expressionStatement. got= "<<intStmt<<std::endl;
                exit(EXIT_FAILURE);
            }

            auto exp = dynamic_cast<prefixExpression*>(intStmt->expressions);
            if(!exp){
                std::cerr<<"stmt not prefixExpression. got= "<<intStmt->expressions<<std::endl;
                exit(EXIT_FAILURE);
            }
            if(exp->operat != input[i].operat){
                std::cerr<<"ident.value not '"<<input[i].operat<<"'. got "
                         <<exp->operat<<std::endl;
                exit(EXIT_FAILURE);
            }
            if(!testIntegerLiteral(exp->right,input[i].integerValue)){
                    return;
            }
        }
        std::cout<< "Everything is okay"<<std::endl;
    }
    return;


}


bool testBooleanLiteral(expression* exp,bool value){
    auto bo = dynamic_cast<boolean*>(exp);
    if(!bo){
        std::cerr<<"exp not boolean. got= "<<typeid(*exp).name()<<std::endl;
        return false;
    }
    if(bo->value != value){
        std::cerr<<" bo.value not "<<value<<". got = "<<bo->value<<std::endl;
    }
    if(bo->tokenLiteral() != std::to_string(value)){
        std::cerr<<"bo.tokenLiteral not "<<value<<". got= "<<bo->tokenLiteral()<<std::endl;
    }

    return true;
}


bool testIdentifier(expression* exp,std::string value){
    auto ident = dynamic_cast<identifier*>(exp);
    if(ident == nullptr){
        std::cerr<<"Exp not identifier*. got= "<<exp<<std::endl;
        return false;
    }
    if(ident->value != value){
        std::cerr<<"ident.value not "<<value 
                 <<". got= "<<ident->value<<std::endl;
        return false;
    }
    if(ident->tokenLiteral() != value){
        std::cerr<<"ident.tokenLiteral not "<<value
                 <<". got= "<<ident->tokenLiteral()<<std::endl;
        return false;
    } 
    return true;
}



bool testLiteralExpression(expression* exp, void* expected){
    if(auto v = static_cast<int*>(expected))
        return testIntegerLiteral(exp,static_cast<int64_t>(*v));
    if(auto v = static_cast<int64_t*>(expected))
        return testIntegerLiteral(exp,*v);
    if(auto v = static_cast<std::string*>(expected))
        return testIdentifier(exp,*v);
    if(auto v = static_cast<bool*>(expected))
        return testBooleanLiteral(exp,*v);
    std::cerr<<"type of exp not handled. got= "<<typeid(*exp).name()<<std::endl;
    return false;
}



bool testInfixExpression(expression* exp,void* left,std::string operat,void* right){
    auto opExp = dynamic_cast<infixExpression*>(exp);
    if(!opExp){
        std::cerr<<"exp is not an infixExpression. got= "<<typeid(*exp).name()<<std::endl;
        return false;
    }
    if(!testLiteralExpression(opExp->left,left)){
        return false;
    }
    if(opExp->operat != operat){
        std::cerr<<"Opeartor is not "<<operat<<". got= "<<opExp->operat<<std::endl;
        return false;
    }
    if(!testLiteralExpression(opExp->right,right)){
        return false;
    }
    return true;
}

void testParsinInfixExpressions(void){
    struct prefixTests{
        std::string input;
        int leftValue;
        std::string operat;
        int rightValue;
    };

    prefixTests input[11] = {
        {"5 + 5;",5,"+",5},
        {"5 - 5;",5,"-",5},
        {"5 * 5;",5,"*",5},
        {"5 / 5;",5,"/",5},
        {"5 > 5;",5,">",5},
        {"5 < 5;",5,"<",5},
        {"5 == 5;",5,"==",5},
        {"5 != 5;",5,"!=",5},
        {"true == true", true, "==", true},
        {"true != false", true, "!=", false},
        {"false == false", false, "==", false},
    };
    
    for(int i=0;i<11;i++){
        lexer lex(input[i].input);
        std::vector<std::string> v;
        parser pars(lex,v);

        program* p = pars.parseProgram(); 
        checkParserErrors(pars);

        if(p->statements.size() !=1){
            std::cerr<<"p.statements doesn't contain 1 statement. got= "
                     << p->statements.size()<<std::endl;
            std::exit(EXIT_FAILURE);

        }

        for(auto stmt : p->statements){
            auto intStmt = dynamic_cast<expressionStatement*>(stmt);
            if(!intStmt){
                std::cerr<<"statements not expressionStatement. got= "<<intStmt<<std::endl;
                exit(EXIT_FAILURE);
            }

            auto exp = dynamic_cast<infixExpression*>(intStmt->expressions);
            if(!exp){
                std::cerr<<"exp not infixExpression. got= "<<intStmt->expressions<<std::endl;
                exit(EXIT_FAILURE);
            }
            if(!testIntegerLiteral(exp->left,input[i].leftValue)){
                    return;
            }
            if(exp->operat != input[i].operat){
                std::cerr<<"exp.operator not '"<<input[i].operat<<"'. got "
                         <<exp->operat<<std::endl;
                exit(EXIT_FAILURE);
            }
            if(!testIntegerLiteral(exp->right,input[i].rightValue)){
                    return;
            }
            if(!testInfixExpression(exp,&input[i].leftValue,input[i].operat,&input[i].rightValue)){
                return;
            }
        }
        std::cout<< "Everything is okay"<<std::endl;
    }
    return;


}

void testOperatorPrecedenceParsing(void){
    struct prefixTests{
        std::string input;
        std::string expected;
    };

    prefixTests input[21] = {
        {"-1 * 2 + 3","(((-1) * 2) + 3)"},
        {"-a * b","((-a) * b)"},
        {"!-a","(!(-a))"},
        {"a + b + c","((a + b) + c)"},
        {"a * b * c","((a * b) * c)"},
        {"a * b / c","((a * b) / c)"},
        {"a + b / c","(a + (b / c))"},
        {"a + b * c + d / e - f","(((a + (b * c)) + (d / e)) - f)"},
        {"3 + 4; -5 * 5","(3 + 4)((-5) * 5)"},
        {"5 > 4 == 3 < 4","((5 > 4) == (3 < 4))"},
        {"5 < 4 != 3 > 4","((5 < 4) != (3 > 4))"},
        {"3 + 4 * 5 == 3 * 1 + 4 * 5","((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"},
        {"true","true"},
        {"false","false"},
        {"3 > 5 == false","((3 > 5) == false)"},
        {"3 < 5 == true","((3 < 5) == true)"},
        {"1 + (2 + 3) + 4","((1 + (2 + 3)) + 4)"},
        {"(5 + 5) * 2","((5 + 5) * 2)"},
        {"2 / (5 + 5)","(2 / (5 + 5))"},
        {"-(5 + 5)","(-(5 + 5))"},
        {"!(true == true)","(!(true == true))"}
    };
    
    for(int i=0;i<21;i++){
        lexer lex(input[i].input);
        std::vector<std::string> v;
        parser pars(lex,v);

        program* p = pars.parseProgram(); 
        checkParserErrors(pars);

        std::string actual = p->strings();
        if(actual != input[i].expected){
            std::cerr<<"expected= "<<input[i].expected<<", got= "
                     <<actual<<std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout<< "Everything is okay"<<std::endl;
    }
    return;
}

int main(void){
    //testLetStatements();
    //testReturnStatements();
    //testIdentifierExpression();
    //testIntegerLiteralExpression();
    //testParsingPrefixExpression();
    //testParsinInfixExpressions();
    testOperatorPrecedenceParsing();
    return 0;
}
