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
    tests input[2]{
        {"5",5},
        {"10",10}
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
    tests input[2]{
        {"true",true},
        {"false",false}
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
    return 0;
}
