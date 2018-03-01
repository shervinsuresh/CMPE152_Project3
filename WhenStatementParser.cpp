/**
 * <h1>WhenStatementParser</h1>

 */
#include <string>
#include <set>
#include "WhenStatementParser.h"
#include "StatementParser.h"
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

set<PascalTokenType> WhenStatementParser::SYM_SET; //need a token name for '=>' symbol

void WhenStatementParser::initialize()
{
    if (INITIALIZED) return;

    SYM_SET = StatementParser::STMT_START_SET;
    SYM_SET.insert(PascalTokenType::SYM);

    set<PascalTokenType>::iterator it;
    for (it  = StatementParser::STMT_FOLLOW_SET.begin();
         it != StatementParser::STMT_FOLLOW_SET.end();
         it++)
    {
        SYM_SET.insert(*it);
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
    token = next_token(token);  // consume the WHEN????

    // Create LOOP, TEST, and NOT nodes.
    ICodeNode *loop_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_LOOP);
    while(token->get_type() != (TokenType) PT_OTHERWISE //ADDED LOOP-ks
	{
	ICodeNode *test_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_TEST);
    ICodeNode *not_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_NOT);

    // The LOOP node adopts the TEST node as its first child.
    // The TEST node adopts the NOT node as its only child.
    loop_node->add_child(test_node);
    test_node->add_child(not_node);

    // Parse the expression.
    // The NOT node adopts the expression subtree as its only child.
    ExpressionParser expression_parser(this);
    not_node->add_child(expression_parser.parse_statement(token));

    // Synchronize at the DO.
    token = synchronize(SYM_SET);
    if (token->get_type() == (TokenType) PT_SYM)
    {
        token = next_token(token);  // consume the =>
    }
    else {
        error_handler.flag(token, MISSING_SYM, this); //MISSING_SYM????
		}
		

    // Parse the statement.
    // The LOOP node adopts the statement subtree as its second child.
    StatementParser statement_parser(this);
    loop_node->add_child(statement_parser.parse_statement(token));
	}//end while
	    // Look for an OTHERWISE.
    if (token->get_type() == (TokenType) PT_OTHERWISE)
    {
        token = next_token(token);  // consume the =>

        // Parse the OTHERWISE statement.
        // The IF node adopts the statement subtree as its third child.
        if_node->add_child(statement_parser.parse_statement(token));
    }

    return loop_node;
}

}}}}  // namespace wci::frontend::pascal::parsers

