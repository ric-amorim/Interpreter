#include "../Object/environment.h"
#include <initializer_list>
#include <vector>


class evaluator {
public:
    object* eval(node* node,environment* env);
    object* evalStatements(std::vector<statement*> stmts,environment* env);
    Boolean* nativeBoolToBooleanObject(bool input);
    object* evalPrefixExpression(std::string operat,object* right);
    object* evalBangOperatorExpression(object* right);
    object* evalMinusPrefixOperatorExpression(object* right);
    object* evalInfixExpression(std::string operat,object* left, object* right);
    object* evalIntegerInfixExpression(std::string operat,object* left, object* right);
    object* evalIfExpression(ifExpression* ie,environment* env);
    bool isTruthy(object* obj);
    object* evalProgram(program* program,environment* env);
    object* evalBlockStatement(blockStatement* block,environment* env);
    Error* newError(std::string format,std::vector<any>); 
    std::string objectToString(objectType t);
    bool isError(object* obj);
    object* evalIdentifier(identifier* id,environment* env);
    std::vector<object*> evalExpressions(std::vector<expression*>,environment*);
    object* applyFunction(object*,std::vector<object*>);
    environment* extendFunctionEnv(Function*,std::vector<object*>);
    object* unwrapReturnValue(object*);

};


