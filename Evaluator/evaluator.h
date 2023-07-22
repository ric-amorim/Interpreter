#include "../Parser/parser.h"
#include "../Object/object.h"
#include <vector>

class evaluator {
public:
    object* eval(node* node);
    object* evalStatements(std::vector<statement*> stmts);
};
