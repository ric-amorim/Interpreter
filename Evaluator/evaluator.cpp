#include "evaluator.h"
#include <vector>

object* evaluator::eval(node* node){
    if (auto intNode = dynamic_cast<integerLiteral*>(node)) {
        return new Integer(intNode->value);
    }
    if(auto expNode = dynamic_cast<expressionStatement*>(node)){
        return eval(expNode->expressions);
    }
    if(auto progNode = dynamic_cast<program*>(node)){
        return evalStatements(progNode->statements);
    }
    
    return nullptr;
}

object* evaluator::evalStatements(std::vector<statement*> stmts){
    object* res;

    for(auto state : stmts){
        res = eval(state);
    } 
    return res;
}
