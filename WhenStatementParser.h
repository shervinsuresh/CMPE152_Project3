/*
 * WhenStatementParser.h
 *
 *  Created on: Feb 27, 2018
 *      Author: shervin
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
     * Parse a WHILE statement.
     * @param token the initial token.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_statement(Token *token) throw (string);

private:
    // Synchronization set for DO.
    static set<PascalTokenType> SYM_SET;

    static bool INITIALIZED;

    /**
     * Initialize the synchronization set.
     */
    static void initialize();
    ICodeNode *parse_branch(Token *token) throw(string);
};

}}}}  // namespace wci::frontend::pascal::parsers

#endif /* WHENSTATEMENTPARSER_H_ */



    
