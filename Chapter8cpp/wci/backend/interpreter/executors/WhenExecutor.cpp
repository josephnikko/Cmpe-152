/**
 * <h1>WhenExecutor</h1>
 *
 * <p>Execute an WHEN statement.</p>
 *
 * Created by Steven Nam D. Le
 */
#include <vector>
#include <map>
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

JumpCache WhenExecutor::jump_cache;

WhenExecutor::WhenExecutor(Executor *parent)
    : StatementExecutor(parent)
{
}

DataValue *WhenExecutor::execute(ICodeNode *node)
{
    // Is there already an entry for this WHEN node in the
    // jump table cache? If not, create a jump table entry.
    JumpTable *jump_table = jump_cache[node];
    if (jump_table == nullptr)
    {
        jump_table = create_jump_table(node);
        jump_cache[node] = jump_table;
    }

    // Get the WHEN node's children.
    vector<ICodeNode *> when_children = node->get_children();


    ExpressionExecutor expression_executor(this);
    DataValue *when_value = expression_executor.execute(expr_node);
    
    
    // If there is a selection, execute the WHEN_BRANCH's statement.
    ICodeNode *statement_node = (*jump_table)[when_value->i];
    if (statement_node != nullptr)
    {
        StatementExecutor statement_executor(this);
        statement_executor.execute(statement_node);
    }

    ++execution_count;  // count the WHEN statement itself
    return nullptr;
}

/**
 * Create a jump table for a WHEN node.
 * @param node the WHEN node.
 * @return the jump table.
 */
JumpTable *WhenExecutor::create_jump_table(ICodeNode *node)
{
    JumpTable *jump_table = new JumpTable();

    // Loop over children that are WHEN_BRANCH nodes.
    vector<ICodeNode *> when_children = node->get_children();
    for (int i = 1; i < when_children.size(); ++i)
    {
        if (i != when_children.size())
        {
            ICodeNode *branch_node = when_children[i];
            ICodeNode *expressions_node = branch_node->get_children()[0];
            ICodeNode *statement_node = branch_node->get_children()[1];
        }
        else //OTHERWISE
        {
            ICodeNode *branch_node = when_children[i];
            ICodeNode *statement_node = branch_node->get_children()[0];
        }

        // Loop over the expressions children of the branch's CONSTANTS_NODE.
        vector<ICodeNode *> expressions_list = expressions_node->get_children();
        for (ICodeNode *expressions_node : expressions_list)
        {
            // Create a jump table entry.               //Need to compare with boolean values?
            NodeValue *node_value =
                    expressions_node->get_attribute((ICodeKey) VALUE);
            DataValue *data_value =  node_value->value;
            (*jump_table)[data_value->i] = statement_node;
        }
    }

    return jump_table;
}


//Note possible debugging errors in the Jump Table. 
}}}}  // namespace wci::backend::interpreter::executors