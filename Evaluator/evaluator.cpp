#include "evaluator.h"
#include <cstdio>
#include <exception>
#include <initializer_list>
#include <sstream>
#include <vector>


Boolean* TRUE  = new Boolean(true);
Boolean* FALSE = new Boolean(false);
Null* NULLS     = new Null();

bool evaluator::isError(object* obj){
    if(obj != nullptr)
        return obj->type() == objectType::error_obj;
    return false;
}


object* evaluator::eval(node* node,environment* env){
    if(auto progNode = dynamic_cast<program*>(node)){
        return evalProgram(progNode,env);
    }
    if(auto blockNode = dynamic_cast<blockStatement*>(node)){
        return evalBlockStatement(blockNode,env);
    }
    if(auto expNode = dynamic_cast<expressionStatement*>(node)){
        return eval(expNode->expressions,env);
    }
    if(auto returnNode = dynamic_cast<returnStatement*>(node)){
        object* val = eval(returnNode->returnValue,env);
        if(isError(val))
            return val;
        return new ReturnValue(val);
    }
    if(auto letNode = dynamic_cast<letStatement*>(node)){
        object* val = eval(letNode->value,env);
        if(isError(val))
            return val;
        env->set(letNode->name->value,val);
    }
    if (auto intNode = dynamic_cast<integerLiteral*>(node)) {
        return new Integer(intNode->value);
    }
    if(auto boolNode = dynamic_cast<boolean*>(node)){
        return nativeBoolToBooleanObject(boolNode->value);
    }
    if(auto prefNode = dynamic_cast<prefixExpression*>(node)){
        object* right = eval(prefNode->right,env);
        if(isError(right))
            return right;
        return evalPrefixExpression(prefNode->operat,right);
    }
    if(auto infixNode = dynamic_cast<infixExpression*>(node)){
        object* left = eval(infixNode->left,env);
        if(isError(left))
            return left;
        object* right = eval(infixNode->right,env);
        if(isError(right))
            return right;
        return evalInfixExpression(infixNode->operat,left,right);
    }
    if(auto ifNode = dynamic_cast<ifExpression*>(node)){
        auto condition = eval(ifNode->condition,env);
        if(isError(condition))
            return condition;
        return evalIfExpression(ifNode,env);
    }
    if(auto identNode = dynamic_cast<identifier*>(node)){
        return evalIdentifier(identNode,env);
    }

    
    return nullptr;
}

object* evaluator::evalIdentifier(identifier* id, environment* env){
    auto val = env->get(id->value);
    if(!val)
        return newError("identifier not found: "+ id->value,{});
    return val;
}

object* evaluator::evalBlockStatement(blockStatement* block,environment* env){
    object* res;

    for(auto statement : block->statements){
        res = eval(statement,env);

        if(res != nullptr){
            auto rt = res->type();
            if(rt == objectType::return_value_obj || rt == objectType::error_obj)
                return res;
        }
    }
    return res;
}

object* evaluator::evalProgram(program* program,environment* env){
    object* res;

    for(auto statement : program->statements){
        res = eval(statement,env);

        if(auto returnValue = dynamic_cast<ReturnValue*>(res))
            return returnValue->value;
        if(auto error = dynamic_cast<Error*>(res)){
            return error;
        }
    }
    return res;
}

object* evaluator::evalIfExpression(ifExpression* ie,environment* env){
    object* condition = eval(ie->condition,env);

    if(isTruthy(condition)){
        return eval(ie->consequence,env);
    }
    if(ie->alternative != nullptr){
        return eval(ie->alternative,env);
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
    if(left->type() != right->type())
        return newError("type mismatch: %s %s %s",{left->type(),operat,right->type()});
    return newError("unknown operator: %s %s %s",{left->type(),operat,right->type()});
    
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
    return newError("unknown operator: %s %s %s",{left->type(),operat,right->type()});
}

object* evaluator::evalPrefixExpression(std::string operat,object* right){
    if(operat == "!")
        return evalBangOperatorExpression(right);
    if(operat == "-")
        return evalMinusPrefixOperatorExpression(right);
    return newError("unknown operator: %s%s",{operat,right->type()});
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
        return newError("unknown operator: -%s",{right->type()});
    Integer* cast = dynamic_cast<Integer*>(right);
    auto value = cast->value;
    return new Integer(-value);
}

object* evaluator::evalStatements(std::vector<statement*> stmts,environment* env){
    object* res;

    for(auto state : stmts){
        res = eval(state,env);

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

std::string replaceString(const std::string& input, const std::string& replacement) {
    std::string result = input;
    size_t pos = 0;
    
    while ((pos = result.find("%s", pos)) != std::string::npos) {
        result.replace(pos, 2, replacement);
        pos += replacement.length();
        return result;

    }
    
    return result;
}

Error* evaluator::newError(std::string format,std::vector<any> args){
    std::stringstream ss;
    std::string res;
    res = format;
    for(auto arg : args){
        try{
            arg.cast<objectType>();
        }catch(const std::exception& e){
            res = replaceString(format,arg.cast<std::string>());
            format = res;
            continue;
        }
        objectType temp = arg.cast<objectType>();
        std::string s = objectToString(temp);
        res = replaceString(format,s);
        format = res;
    }
    ss<<res;
    return new Error(ss.str());

}

std::string evaluator::objectToString(objectType t){
    std::map<objectType,std::string> s{
        {objectType::null_obj, "NULL"},
        {objectType::error_obj, "ERROR"},
        {objectType::boolean_oj, "BOOLEAN"},
        {objectType::integer_obj, "INTEGER"},
        {objectType::return_value_obj,"RETURN"},
    };
    auto it = s.find(t);
    if (it != s.end()) {
        return it->second;
    } else {
        return "unknown";
    }
}

