/**
 * <h1>WhenStatementParser</h1>
 *
 * <p>Parse a Pascal WHEN statement.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
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

set<PascalTokenType> WhenStatementParser::RIGHT_ARROW_SET;

void WhenStatementParser::initialize()
{
    if (INITIALIZED) return;

    RIGHT_ARROW_SET = StatementParser::STMT_START_SET;
    RIGHT_ARROW_SET.insert(PascalTokenType::RIGHT_ARROW);

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

    // Create a WHEN node.
    ICodeNode *when_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_WHEN);
     
    // Create a WHEN_BRANCH node.
    ICodeNode *when_branch_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_WHEN_BRANCH);       
            
    // Loop to parse each WHEN_BRANCH until the OTHERWISE token
    // or the end of the source file.
    
    while ((token != nullptr) &&
           (token->get_type() != (TokenType) PT_OTHERWISE))
    {
        // The WHEN node adopts the WHEN_BRANCH subtree.
        when_node->add_child(when_branch_node);
        
        // Parse the expression.
   		// The WHEN_BRANCH node adopts the expression subtree as its first child.
    	ExpressionParser expression_parser(this);
    	when_branch_node->add_child(expression_parser.parse_statement(token));
		
		// Synchronize at the RIGHT_ARROW.
  	  	token = synchronize(RIGHT_ARROW_SET);
    	if (token->get_type() == (TokenType) PT_RIGHT_ARROW)
    	{
   		     token = next_token(token);  // consume the RIGHT_ARROW
   		}
  /*	else {
        	error_handler.flag(token, MISSING_OF, this);
    	}
	*/	
		
		// Parse the statement.
		StatementParser statement_parser(this);
		when_branch_node->add_child(statement_parser.parse_statement(token));
		
        token = current_token();
        TokenType token_type = token->get_type();

        // Look for the semicolon between WHEN branches.
        if (token_type == (TokenType) PT_SEMICOLON)
        {
            token = next_token(token);  // consume the ;
        }

        // If at the start of the next constant, then missing a semicolon.
        /*
        else if (CONSTANT_START_SET.find((PascalTokenType) token_type)
                      != CONSTANT_START_SET.end())
        {
            error_handler.flag(token, MISSING_SEMICOLON, this);
        } */
    }

	// Create an OTHERWISE node.
    ICodeNode *otherwise_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_OTHERWISE);

	token = next_token(token);	//consume OTHERWISE
	
	// Parse the statement.
	StatementParser otherwise_statement_parser(this);
	otherwise_node->add_child(otherwise_statement_parser.parse_statement(token));
	
	token = current_token();
	
	// Look for the END token.
    if (token->get_type() == (TokenType) PT_END)
    {
        token = next_token(token);  // consume END
    }
    else
    {
        error_handler.flag(token, MISSING_END, this);
    }

    return when_node;
}




}}}}  // namespace wci::frontend::pascal::parsers
