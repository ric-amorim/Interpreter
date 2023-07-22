#include "../Parser/parser.h"
#include "../Object/object.h"
#include <vector>

class evaluator {
public:
    object* eval(node* node);
    object* evalStatements(std::vector<statement*> stmts);
    Boolean* nativeBoolToBooleanObject(bool input);
    object* evalPrefixExpression(std::string operat,object* right);
    object* evalBangOperatorExpression(object* right);
    object* evalMinusPrefixOperatorExpression(object* right);
};
