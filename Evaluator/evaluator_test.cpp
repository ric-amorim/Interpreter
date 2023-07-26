#include "evaluator.h"
#include <cstdlib>


object* testEval(std::string input){
    lexer lex(input);
    std::vector<std::string> v;
    parser pars(lex,v);
    program* p = pars.parseProgram(); 
    
    evaluator eval;
    return eval.eval(p);
}

bool testIntegerObject(object* obj,int expected){
    auto result = dynamic_cast<Integer*>(obj);
    if(!result){
        std::cerr<<"object is not Integer. got= "<<typeid(obj).name()
                 <<" ("<<obj->inspect()<<")"<<std::endl;
        return false;
    }
    if(result->value != expected){
        std::cerr<<"object has wrong value. got= "<<result->value
                 <<", want= "<<expected<<std::endl;
        return false;
    }
    return true;
}

void testEvalIntegerExpression(){
    struct tests{
        std::string input;
        int expected;
    };
    tests input[15]{
        {"5",5},
        {"10",10},
        {"-5",-5},
        {"-10",-10},
        {"5 + 5 + 5 + 5 - 10",10},
        {"2 * 2 * 2 * 2 * 2",32},
        {"-50 + 100 + -50",0},
        {"5 * 2 + 10",20},
        {"5 + 2 * 10",25},
        {"20 + 2 * -10",0},
        {"50 / 2 * 2 + 10",60},
        {"2 * (5 + 10)",30},
        {"3 * 3 * 3 + 10",37},
        {"3 * (3 * 3) + 10",37},
        {"(5 + 10 * 2 + 15 / 3) * 2 + -10",50},
    };
    for(tests tt : input){
        object* evaluated = testEval(tt.input);
        testIntegerObject(evaluated,tt.expected);
    }
    std::cout<<"Everything is okay!"<<std::endl;
}

bool testBooleanObject(object* obj,bool expected){
    auto res = dynamic_cast<Boolean*>(obj);
    if(!res){
        std::cerr<<"object is not Boolean.got = "<<typeid(obj).name()
                 <<" ("<<obj->inspect()<<")"<<std::endl;
        return false;
    }
    if(res->value != expected){
        std::cerr<<"object has wrong value. got= "<<res->value
                 <<", want= "<<expected<<std::endl;
        return false;
    }
    return true;

}

void testEvalBooleanExpression(){
    struct tests{
        std::string input;
        bool expected;
    };
    tests input[19]{
        {"true",true},
        {"false",false},
        {"1 < 2", true},
        {"1 > 2", false},
        {"1 < 1", false},
        {"1 > 1", false},
        {"1 == 1", true},
        {"1 != 1", false},
        {"1 == 2", false},
        {"1 != 2", true},
        {"true == true", true},
        {"false == false", true},
        {"true == false", false},
        {"true != false", true},
        {"false != true", true},
        {"(1 < 2) == true", true},
        {"(1 < 2) == false", false},
        {"(1 > 2) == true", false},
        {"(1 > 2) == false", true},
    };
    for(tests tt : input){
        object* evaluated = testEval(tt.input);
        testBooleanObject(evaluated,tt.expected);
    }
    std::cout<<"Everything is okay!"<<std::endl;
}

void testBangOperator(){
    struct tests{
        std::string input;
        bool expected;
    };
    tests input[6]{
        {"!true",false},
        {"!false",true},
        {"!5",false},
        {"!!true",true},
        {"!!false",false},
        {"!!5",true}
    };
    for(tests tt : input){
        object* evaluated = testEval(tt.input);
        testBooleanObject(evaluated,tt.expected);
    }
    std::cout<<"Everything is okay!"<<std::endl;

}

int main(){
    //testEvalIntegerExpression();
    testEvalBooleanExpression();
    //testBangOperator();
    return 0;
}
