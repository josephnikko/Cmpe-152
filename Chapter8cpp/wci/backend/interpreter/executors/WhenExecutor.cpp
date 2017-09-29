/**
 * <h1>WhenExecutor</h1>
 *
 * <p>Execute an WHEN statement.</p>
 *
 * Created by Steven Nam D. Le
 */
#include <vector>
#include "WhenExecutor.h"
#include "StatementExecutor.h"
#include "ExpressionExecutor.h"
#include "../../../DataValue.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;
using namespace wci::intermediate::icodeimpl;

IfExecutor::IfExecutor(Executor *parent)
    : StatementExecutor(parent)
{
}

DataValue *WhenExecutor::execute(ICodeNode *node)
{
    // Get the WHEN node's children.
    //NOTE TO SELF: EXPRESSION, THEN, ELSE STATEMENT IS LISTED HERE. 
    
    vector<ICodeNode *> children = node->get_children();
    ICodeNode *expr_node = children[0]; 
    ICodeNode *when_branch_stmt_node = children[1];
    ICodeNode *otherwise_stmt_node = children.size() > 2 ? children[2] : nullptr;

    ExpressionExecutor expression_executor(this);
    StatementExecutor statement_executor(this);

    // Evaluate the expression to determine which statement to execute.
    DataValue *data_value = expression_executor.execute(expr_node);
    if (data_value->b) //this is a boolean
    {
        statement_executor.execute(when_branch_stmt_node);
    }
    else if (otherwise_stmt_node != nullptr) //Runs this statement if a boolean is not found. 
    {
        statement_executor.execute(otherwise_stmt_node); //Should this be an expression?
    }
    
    ++execution_count;  // count the WHEN statement itself
    return nullptr;
}

}}}}  // namespace wci::backend::interpreter::executors
