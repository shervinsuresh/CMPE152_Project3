/*
 * WhenStatementParser.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: shervin
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

set<PascalTokenType> WhenStatementParser::THEN_SET;

void WhenStatementParser::initialize()
{
    if (INITIALIZED) return;

    THEN_SET = StatementParser::STMT_START_SET;
    THEN_SET.insert(PascalTokenType::THEN);

    set<PascalTokenType>::iterator it;
    for (it  = StatementParser::STMT_FOLLOW_SET.begin();
         it != StatementParser::STMT_FOLLOW_SET.end();
         it++)
    {
        THEN_SET.insert(*it);
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
    token = next_token(token);  // consume the IF

    // Create an IF node.
    ICodeNode *if_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_IF);


    // Parse the expression.
    // The IF node adopts the expression subtree as its first child.
    ExpressionParser expression_parser(this);
    if_node->add_child(expression_parser.parse_statement(token));

    // Synchronize at the THEN.
    token = synchronize(THEN_SET);
    if (token->get_type() == (TokenType) PT_THEN)
    {
        token = next_token(token);  // consume the THEN
    }
    else {
        error_handler.flag(token, MISSING_THEN, this);
    }

    // Parse the THEN statement.
    // The IF node adopts the statement subtree as its second child.
    StatementParser statement_parser(this);
    if_node->add_child(statement_parser.parse_statement(token));
    token = current_token();

    // Look for an ELSE.
    if (token->get_type() == (TokenType) PT_ELSE)
    {
        token = next_token(token);  // consume the THEN

        // Parse the ELSE statement.
        // The IF node adopts the statement subtree as its third child.
        if_node->add_child(statement_parser.parse_statement(token));
    }

    return if_node;
}

}}}}  // namespace wci::frontend::pascal::parsers



