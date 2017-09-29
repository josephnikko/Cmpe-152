/**
 * <h1>WhenStatementParser</h1>
 *
 * <p>Parse a Pascal WHEN statement.</p>
 *
 * Edited by Steven Nam D. Le
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include <set>
#include "WhenStatementParser.h"
#include "StatementParser.h"
#include "AssignmentStatementParser.h"
#include "ExpressionParser.h"
#include "../PascalParserTD.h"
#include "../PascalToken.h"
#include "../PascalError.h"
#include "../../Token.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/ICodeFactory.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;
using namespace wci::intermediate::icodeimpl;

bool WhenStatementParser::INITIALIZED = false;

set<PascalTokenType> WhenStatementParser::RIGHT_ARROW_SET;                     //<-- Edit this

void WhenStatementParser::initialize()                                  //<-- Edit this
{
    if (INITIALIZED) return;

    RIGHT_ARROW_SET = StatementParser::STMT_START_SET;
    RIGHT_ARROW_SET.insert(PascalTokenType::THEN);                             //<-- Edit this. See Lab Slides 09/12/17

    set<PascalTokenType>::iterator it;
    for (it  = StatementParser::STMT_FOLLOW_SET.begin();
         it != StatementParser::STMT_FOLLOW_SET.end();
         it++)
    {
        RIGHT_ARROW_SET.insert(*it);
    }

    INITIALIZED = true;
}

WhenStatementParser::WhenStatementParser(PascalParserTD *parent)
    : StatementParser(parent)
{
    initialize();
}

ICodeNode *WhenStatementParser::parse_statement(Token *token) throw (string)
{
    token = next_token(token);  // consume the WHEN

    // Create an WHEN node.
    ICodeNode *when_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_WHEN);


    // Parse the expression.
    // The WHEN node adopts the expression subtree as its first child.
    ExpressionParser expression_parser(this);
    when_node->add_child(expression_parser.parse_statement(token));

    // Synchronize at the THEN.
    token = synchronize(RIGHT_ARROW_SET);
    if (token->get_type() == (TokenType) PT_RIGHT_ARROW) //EDIT THIS PT_RIGHT_ARROW
    {
        token = next_token(token);  // consume the THEN
    }

    else {
        error_handler.flag(token, MISSING_RIGHT_ARROW, this); //<--------HAS THIS BEEN CREATED?
    }

    // Parse the THEN statement.                                           <-- EDIT THIS
    // The WHEN node adopts the statement subtree as its second child.
    StatementParser statement_parser(this);
    when_node->add_child(statement_parser.parse_statement(token));
    token = current_token();

    // Look for an ELSE.                                                   <-- EDIT THIS
    if (token->get_type() == (TokenType) PT_OTHERWISE) //EDIT THIS
    {
        token = next_token(token);  // consume the THEN

        // Parse the ELSE statement.
        // The WHEN node adopts the statement subtree as its third child.
        when_node->add_child(statement_parser.parse_statement(token));
    }
        else {
        error_handler.flag(token, MISSING_OTHERWISE, this); //<--------HAS THIS BEEN CREATED?
    }

    return when_node;
}

}}}}  // namespace wci::frontend::pascal::parsers
