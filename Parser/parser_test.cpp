#include "parser.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

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
            std::cerr<<"exp not integerLiteral . got= "<<intStmt->expressions<<std::endl;
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
        std::cerr<<"exp not integerLiteral. got= "<<typeid(*il).name()<<std::endl;
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



bool testLiteralExpression(expression* exp, const any& expected){
    if(expected.isType<int>()){
        int v = expected.cast<int>();
        return testIntegerLiteral(exp,v);
    }
    if(expected.isType<std::string>()){
        std::string v = expected.cast<std::string>();
        return testIdentifier(exp,v);
    }
    if(expected.isType<bool>()){
        bool v = expected.cast<bool>();
        return testBooleanLiteral(exp,v);
    }
    std::cerr<<"type of exp not handled. got= "<<typeid(*exp).name()<<std::endl;
    return false;
}



bool testInfixExpression(expression* exp,const any& left,std::string operat,const any& right){
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

    prefixTests input[24] = {
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
        {"!(true == true)","(!(true == true))"},
        {"a + add(b * c) + d","((a + add((b * c))) + d)"},
        {"add(a, b, 1, 2 * 3, 4 + 5, add(6, 7 * 8))","add(a, b, 1, (2 * 3), (4 + 5), add(6, (7 * 8)))"},
        {"add(a + b + c * d / f + g)","add((((a + b) + ((c * d) / f)) + g))"}
    };
    
    for(int i=0;i<24;i++){
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

void testIfExpression(void){
    std::string input = {"if (x < y) { x }"};

    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);

    program* p = pars.parseProgram();
    checkParserErrors(pars);

    if(p->statements.size() != 1){
        std::cerr<<"program.statements does not contain "<<1<<". got= "
                 <<p->statements.size()<<std::endl;
        exit(EXIT_FAILURE);
    }
    auto stmt = dynamic_cast<expressionStatement*>(p->statements[0]);
    if(!stmt){
        std::cerr<<"program.statements[0] is not expressionStatement. got= "
                 <<p->statements[0]<<std::endl;
        exit(EXIT_FAILURE);
    }

    auto exp = dynamic_cast<ifExpression*>(stmt->expressions);
    if(!exp){
        std::cerr<<"stmt.expression is not ifExpression. got= "
            <<stmt->expressions<<std::endl;
        exit(EXIT_FAILURE);
    }
    any l = std::string("x");
    any r = std::string("y");
    if(!testInfixExpression(exp->condition,l,"<",r))
            return;
    if(exp->consequence->statements.size() != 1){
        std::cerr<<"consequence is not 1 statements. got= "
                 <<exp->consequence->statements.size()<<std::endl;
        exit(EXIT_FAILURE);
    }

    auto consequence = dynamic_cast<expressionStatement*>(exp->consequence->statements[0]);
    if(!consequence){
        std::cerr<<"statements[0] is not expressionStatement. got= "
                 <<exp->consequence->statements[0]<<std::endl;
        exit(EXIT_FAILURE);
    } 
    if(!testIdentifier(consequence->expressions,"x"))
        return;
    if(exp->alternative != nullptr){
        std::cerr<<"exp.alternative.statements was not null.got= "
                 <<exp->alternative<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout<< "Everything is okay"<<std::endl;
}

void testIfElseExpression(void){
    std::string input = {"if (x < y) { x } else { y }"};

    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);

    program* p = pars.parseProgram();
    checkParserErrors(pars);

    if(p->statements.size() != 1){
        std::cerr<<"program.statements does not contain "<<1<<". got= "
                 <<p->statements.size()<<std::endl;
        exit(EXIT_FAILURE);
    }
    auto stmt = dynamic_cast<expressionStatement*>(p->statements[0]);
    if(!stmt){
        std::cerr<<"program.statements[0] is not expressionStatement. got= "
                 <<p->statements[0]<<std::endl;
        exit(EXIT_FAILURE);
    }

    auto exp = dynamic_cast<ifExpression*>(stmt->expressions);
    if(!exp){
        std::cerr<<"stmt.expression is not ifExpression. got= "
            <<stmt->expressions<<std::endl;
        exit(EXIT_FAILURE);
    }
    any l = std::string("x");
    any r = std::string("y");
    if(!testInfixExpression(exp->condition,l,"<",r))
            return;
    if(exp->consequence->statements.size() != 1){
        std::cerr<<"consequence is not 1 statements. got= "
                 <<exp->consequence->statements.size()<<std::endl;
        exit(EXIT_FAILURE);
    }

    auto consequence = dynamic_cast<expressionStatement*>(exp->consequence->statements[0]);
    if(!consequence){
        std::cerr<<"statements[0] is not expressionStatement. got= "
                 <<exp->consequence->statements[0]<<std::endl;
        exit(EXIT_FAILURE);
    } 
    if(!testIdentifier(consequence->expressions,"x"))
        return;

    if(exp->alternative->statements.size()!= 1){
        std::cerr<<"exp.alternative.statements does not contain 1 statement.got= "
                 <<exp->alternative->statements.size()<<std::endl;
        exit(EXIT_FAILURE);
    }

    auto alternative = dynamic_cast<expressionStatement*>(exp->alternative->statements[0]); 
    if (!alternative) {
        std::cerr<<"Statements[0] is not expressionStatement. got= "
			<<exp->alternative->statements[0]<<std::endl;
        exit(EXIT_FAILURE);
	}

	if (!testIdentifier(alternative->expressions, "y"))
		return;

    std::cout<< "Everything is okay"<<std::endl;
}

void testFunctionLiteralParsing(void){
    std::string input = {"fn(x, y) {x + y; }"};

    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);

    program* p = pars.parseProgram();
    checkParserErrors(pars);

    if(p->statements.size() != 1){
        std::cerr<<" program.statements does not contain "<<1<<" statements. got= "
                 <<p->statements.size()<<std::endl;
        exit(EXIT_FAILURE);
    }
    
    auto stmt = dynamic_cast<expressionStatement*>(p->statements[0]);
    if(!stmt){
        std::cerr<<" program.statements[0] is not expressionsStatements. got= "
                 <<p->statements[0]<<std::endl;
        exit(EXIT_FAILURE);
    }

    auto function = dynamic_cast<functionLiteral*>(stmt->expressions);
    if(!function){
        std::cerr<<"stmt.expression is not functionLiteral. got= "
                 <<stmt->expressions<<std::endl;
        exit(EXIT_FAILURE);
    }
    if(function->parameters.size() != 2){
        std::cerr<<"functionLiteral parameters wrong. want 2, got= "
                 <<function->parameters.size()<<std::endl;
        exit(EXIT_FAILURE);
    }
    any x = std::string("x");
    any y = std::string("y");
    testLiteralExpression(function->parameters[0],x);
    testLiteralExpression(function->parameters[1],y);

    if(function->body->statements.size() != 1){
        std::cerr<<"function.body.statements has not 1 statements, got= "
                 <<function->body->statements.size()<<std::endl;
        exit(EXIT_FAILURE);
    }

    auto bodyStmt = dynamic_cast<expressionStatement*>(function->body->statements[0]);
    if(!bodyStmt){
        std::cerr<<"function.body.statements is not expressionStatement, got= "
                 <<function->body->statements[0]<<std::endl;
        exit(EXIT_FAILURE);
    }    

    testInfixExpression(bodyStmt->expressions,x,"+",y);
    
    std::cout<< "Everything is okay"<<std::endl;
}

void testFunctionParameterParsing(void){
    struct tests{
        std::string input;
        std::vector<std::string> expectedParams;
    };

    tests input[3] = {{"fn() {};",{}},
                      {"fn(x) {};",{"x"}},                  
                      {"fn(x, y, z) {};",{"x","y","z"}}
    };

    for(tests tt : input){
        lexer lex(tt.input);
        std::vector<std::string> v;
        parser pars(lex,v);

        program* p = pars.parseProgram();
        checkParserErrors(pars);

        auto stmt = dynamic_cast<expressionStatement*>(p->statements[0]);
        auto function = dynamic_cast<functionLiteral*>(stmt->expressions);

        if(function->parameters.size() != tt.expectedParams.size()){
            std::cerr<<"length parameters wrong. want "
                     <<tt.expectedParams.size()<<", got= "
                     <<function->parameters.size()<<std::endl;
            exit(EXIT_FAILURE);
        }
        int i=0;
        for(auto ident : tt.expectedParams){
            testLiteralExpression(function->parameters[i],ident);
            i++;
        }
    
        std::cout<< "Everything is okay"<<std::endl;

    }


}

void testCallExpressionParsing(void){
    std::string input = {"add(1, 2 * 3, 4 + 5);"};

    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);

    program* p = pars.parseProgram();
    checkParserErrors(pars);

    if(p->statements.size() != 1){
        std::cerr<<" program.statements does not contain "<<1<<" statements. got= "
                 <<p->statements.size()<<std::endl;
        exit(EXIT_FAILURE);
    }

    auto stmt = dynamic_cast<expressionStatement*>(p->statements[0]);
    if(!stmt){
        std::cerr<<"statements is not expressionStatement. got= "
                 <<p->statements[0]<<std::endl;
        exit(EXIT_FAILURE);
    }
    auto exp = dynamic_cast<callExpression*>(stmt->expressions);
    if(!exp){
        std::cerr<<"stmt.expression is not callExpression. got= "
                 <<stmt->expressions<<std::endl;
        exit(EXIT_FAILURE);
    }
    if(!testIdentifier(exp->function,"add"))
        return;

    if(exp->arguments.size() != 3){
        std::cerr<<"wrong length of arguments. got= "
                 <<exp->arguments.size()<<std::endl;
        exit(EXIT_FAILURE);

    }
    testLiteralExpression(exp->arguments[0],1);
    testInfixExpression(exp->arguments[1],2,"*",3);
    testInfixExpression(exp->arguments[2],4,"+",5);

    std::cout<< "Everything is okay"<<std::endl;
    
}

void testCallExpressionParameterParsing(void){
    struct tests{
        std::string input;
        std::string expectedIdent;
        std::vector<std::string> expectedArgs;

    };

    tests input[3] = {{"add();","add",{}},
                      {"add(1);","add",{"1"}},
                      {"add(1, 2 * 3, 4 + 5);","add",{"1","(2 * 3)","(4 + 5)"}}
    };

    for(auto tt : input){
        lexer lex(tt.input);
        std::vector<std::string> v;
        parser pars(lex,v);

        program* p = pars.parseProgram();
        checkParserErrors(pars);

        auto stmt = dynamic_cast<expressionStatement*>(p->statements[0]);
        auto exp = dynamic_cast<callExpression*>(stmt->expressions);

        if(!exp){
            std::cerr<<"stmt.expression is not callExpression.got= "
                     <<stmt->expressions<<std::endl;
            exit(EXIT_FAILURE);
        }
        if(!testIdentifier(exp->function,tt.expectedIdent))
            return;
        if(exp->arguments.size() != tt.expectedArgs.size()){
            std::cerr<<"wrong number of args. want= "<<tt.expectedArgs.size()
                     <<",got= "<<exp->arguments.size()<<std::endl;
            exit(EXIT_FAILURE);
        }

        int i=0;
        for(auto ident : tt.expectedArgs){
            if(exp->arguments[i]->strings() != ident){
                std::cerr<<"argument "<<i<<" wrong. want = "
                         <<ident<<",got= "<<exp->arguments[i]->strings();
                exit(EXIT_FAILURE);
            }
            i++;
        }
    
        std::cout<< "Everything is okay"<<std::endl;

    }
}

void testLetStatements(void){
    struct tests{
        std::string input;
        std::string expectedIdentifier;
        any expectedValue;
    };
    any y = std::string("y");

    tests input[3]{{"let x = 5;","x",5},
                   {"let y = true;","y",true},
                   {"let foobar = y;","foobar",y}
    };

    for(auto tt : input){
        lexer lex(tt.input);
        std::vector<std::string> v;
        parser pars(lex,v);

        program* p = pars.parseProgram(); 
        checkParserErrors(pars);
        if(p->statements.size() != 1) {
            std::cerr<<"p.statements doesn't contain 1 statements. got= "
                     << p->statements.size()<<std::endl;
            std::exit(EXIT_FAILURE);
        }
        auto stmt = p->statements[0];
        if(!testLetStatement(stmt,tt.expectedIdentifier))
            return;

        letStatement* letStmt = dynamic_cast<letStatement*>(stmt);
        expression* val = letStmt->value;
        if(!testLiteralExpression(val,tt.expectedValue))
            return;

        std::cout<< "Everything is okay"<<std::endl;
    }
}


void testReturnStatements(void){
    struct tests{
        std::string input;
        any expectedValue;
    };
    any foobar = std::string("foobar");
    tests input[3]={{"return 5;",5},
                   {"return true;",true},
                   {"return foobar;",foobar}
    };

    for(auto tt : input){
        lexer lex(tt.input);
        std::vector<std::string> v;
        parser pars(lex,v);

        program* p = pars.parseProgram(); 
        checkParserErrors(pars);
        if(p->statements.size() != 1) {
            std::cerr<<"p.statements doesn't contain 1 statements. got= "
                     << p->statements.size()<<std::endl;
            std::exit(EXIT_FAILURE);
        }
        auto stmt = p->statements[0];

        returnStatement* returnStmt = dynamic_cast<returnStatement*>(stmt);
        if(!returnStmt){
            std::cerr<<"stmt not returnStmt. got= "<<stmt<<std::endl;
            exit(EXIT_FAILURE);
        }
        if(returnStmt->tokenLiteral() != "return") {
            std::cerr<<"returnStmt.tokenLiteral not 'return',got "
                     <<returnStmt->tokenLiteral()<<std::endl;
            exit(EXIT_FAILURE);
        }

        if(!testLiteralExpression(returnStmt->returnValue,tt.expectedValue))
            return;

        std::cout<< "Everything is okay"<<std::endl;
    }
}
int main(void){
    testLetStatements();
    testReturnStatements();
    //testIdentifierExpression();
    //testIntegerLiteralExpression();
    //testParsingPrefixExpression();
    //testParsinInfixExpressions();
    //testOperatorPrecedenceParsing();
    //testIfExpression();
    //testIfElseExpression();
    //testFunctionLiteralParsing();
    //testFunctionParameterParsing();
    //testCallExpressionParsing();
    //testCallExpressionParameterParsing();
    return 0;
}
