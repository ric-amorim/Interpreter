#include "evaluator.h"
#include <vector>


Boolean* TRUE  = new Boolean(true);
Boolean* FALSE = new Boolean(false);
Null* NULLS     = new Null();


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
    if(auto boolNode = dynamic_cast<boolean*>(node)){
        return nativeBoolToBooleanObject(boolNode->value);
    }
    if(auto prefNode = dynamic_cast<prefixExpression*>(node)){
        object* right = eval(prefNode->right);
        return evalPrefixExpression(prefNode->operat,right);
    }
    
    return nullptr;
}

object* evaluator::evalPrefixExpression(std::string operat,object* right){
    if(operat == "!")
        return evalBangOperatorExpression(right);
    if(operat == "-")
        return evalMinusPrefixOperatorExpression(right);
    return NULLS;
}

object* evaluator::evalBangOperatorExpression(object* right){
    if(right == TRUE)
        return FALSE;
    if(right == FALSE)
        return TRUE;
    if(right == NULLS)
        return TRUE;
    return FALSE;
}

object* evaluator::evalMinusPrefixOperatorExpression(object* right){
    if(right->type() != integer_obj)
        return NULLS;
    Integer* cast = dynamic_cast<Integer*>(right);
    auto value = cast->value;
    return new Integer(-value);
}

object* evaluator::evalStatements(std::vector<statement*> stmts){
    object* res;

    for(auto state : stmts){
        res = eval(state);
    } 
    return res;
}

Boolean* evaluator::nativeBoolToBooleanObject(bool input){
    if(input)
        return TRUE;
    return FALSE;
}
