/**
 * <h1>WhenStatementParser</h1>
 *
 * <p>Parse a Pascal WHEN statement.</p>
 *
 * Edited by Steven Nam D. Le
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef WHENSTATEMENTPARSER_H_
#define WHENSTATEMENTPARSER_H_

#include <string>
#include <set>
#include "StatementParser.h"
#include "../../Token.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;

class WhenStatementParser : public StatementParser
{
public:
    /**
     * Constructor.
     * @param parent the parent parser.
     */
    WhenStatementParser(PascalParserTD *parent);

    /**
     * Parse an WHEN statement.
     * @param token the initial token.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_statement(Token *token) throw (string);

private:
    // Synchronization set for THEN.
    static set<PascalTokenType> THEN_SET;       //<---------- NEEDS TO BE UPDATED TO INCLUDE OTHERWISE.

    static bool INITIALIZED;

    /**
     * Initialize the synchronization set.
     */
    static void initialize();
};

}}}}  // namespace wci::frontend::pascal::parsers

#endif /* WHENSTATEMENTPARSER_H_ */
