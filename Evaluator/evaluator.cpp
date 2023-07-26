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
    if(auto infixNode = dynamic_cast<infixExpression*>(node)){
        object* left = eval(infixNode->left);
        object* right = eval(infixNode->right);
        return evalInfixExpression(infixNode->operat,left,right);
    }
    if(auto blockNode = dynamic_cast<blockStatement*>(node)){
        return evalBlockStatement(blockNode);
    }
    if(auto ifNode = dynamic_cast<ifExpression*>(node)){
        return evalIfExpression(ifNode);
    }
    if(auto returnNode = dynamic_cast<returnStatement*>(node)){
        object* val = eval(returnNode->returnValue);
        return new ReturnValue(val);
    }
    if(auto progNode = dynamic_cast<program*>(node)){
        return evalProgram(progNode);
    }

    
    return nullptr;
}

object* evaluator::evalBlockStatement(blockStatement* block){
    object* res;

    for(auto statement : block->statements){
        res = eval(statement);

        if(res != nullptr && res->type() == objectType::return_value_obj)
            return res;
    }
    return res;
}

object* evaluator::evalProgram(program* program){
    object* res;

    for(auto statement : program->statements){
        res = eval(statement);

        if(auto returnValue = dynamic_cast<ReturnValue*>(res))
            return returnValue->value;
    }
    return res;
}

object* evaluator::evalIfExpression(ifExpression* ie){
    object* condition = eval(ie->condition);

    if(isTruthy(condition)){
        return eval(ie->consequence);
    }
    if(ie->alternative != nullptr){
        return eval(ie->alternative);
    }
    return NULLS;
}

bool evaluator::isTruthy(object* obj){
    if(obj == NULLS)
        return false;
    if(obj == TRUE)
        return true;
    if(obj == FALSE)
        return false;
    return true;
}

object* evaluator::evalInfixExpression(std::string operat,object* left,object* right){
    if(left->type() == objectType::integer_obj && right->type() == objectType::integer_obj)
        return evalIntegerInfixExpression(operat,left,right);
    if(operat == "==")
        return nativeBoolToBooleanObject(left == right);
    if(operat == "!=")
        return nativeBoolToBooleanObject(left != right);
    return NULLS;
    
}

object* evaluator::evalIntegerInfixExpression(std::string operat, object* left,object* right){
    Integer* leftTemp = dynamic_cast<Integer*>(left);
    auto leftVal = leftTemp->value;

    Integer* rightTemp = dynamic_cast<Integer*>(right);
    auto rightVal = rightTemp->value;

    if(operat == "+"){
        return new Integer(leftVal + rightVal);
    }
    if(operat == "-"){
        return new Integer(leftVal - rightVal);
    }
    if(operat == "*"){
        return new Integer(leftVal * rightVal);
    }
    if(operat == "/"){
        return new Integer(leftVal / rightVal);
    }
    if(operat == "<"){
        return nativeBoolToBooleanObject(leftVal < rightVal);
    }
    if(operat == ">"){
        return nativeBoolToBooleanObject(leftVal > rightVal);
    }
    if(operat == "=="){
        return nativeBoolToBooleanObject(leftVal == rightVal);
    }
    if(operat == "!="){
        return nativeBoolToBooleanObject(leftVal != rightVal);
    }
    return NULLS;
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

        if(auto returnValue = dynamic_cast<ReturnValue*>(res))
            return returnValue->value;
    } 
    return res;
}

Boolean* evaluator::nativeBoolToBooleanObject(bool input){
    if(input)
        return TRUE;
    return FALSE;
}
