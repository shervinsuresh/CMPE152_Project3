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
    ICodeNode *when_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_WHEN);
    while((token->get_type() != (TokenType) PT_OTHERWISE) && (token->get_type() != (TokenType) PT_END) && (token != nullptr)) //ADDED LOOP-ks
	{
    		when_node->add_child(parse_branch(token));

    		token = current_token();
    	  	TokenType token_type = token->get_type();

    	   	if (token_type == (TokenType) PT_SEMICOLON)
    	    {
    	   		token = next_token(token); //consume the ;
    	  	}

    	   	else
    	  	{
    	      	error_handler.flag(token, MISSING_SEMICOLON, this);
    		}
	}//end while

	    // Look for an OTHERWISE.
    if (token->get_type() == (TokenType) PT_OTHERWISE)
    {
        token = next_token(token);  // consume the otherwise

        // Parse the OTHERWISE statement.
        // The IF node adopts the statement subtree as its third child.
        token = synchronize(SYM_SET);
        if (token->get_type()==(TokenType) PT_SYM)
        {
        		token = next_token(token); //consume the =>
        }

        else
        {
        		error_handler.flag(token, MISSING_SYM, this);
        }
        ICodeNode *otherwise_node =
                        ICodeFactory::create_icode_node((ICodeNodeType) NT_OTHERWISE);
        StatementParser statementParser(this);
        otherwise_node->add_child(statementParser.parse_statement(token));
        when_node->add_child(otherwise_node);
    }

    else
    {
      	error_handler.flag(token, MISSING_OTHERWISE, this);
    }

    if (token->get_type() == (TokenType) PT_END)
       {
         token = next_token(token); //consume END
       }
       else
       {
         error_handler.flag(token, MISSING_END, this);
       }
}

ICodeNode *WhenStatementParser::parse_branch(Token *token) throw (string)
{
  //create a WHEN_BRANCH node EQ and ASSIGN node
  //the WHEN_BRANCH adopts the EQ node as it's first child
  //and the ASSIGN branch as it's second child
  ICodeNode *branch_node =
            ICodeFactory::create_icode_node(
                  (ICodeNodeType) NT_WHEN_BRANCH);
  //parse the expression
  // the branch_node adopts the expression subtree as it's first child
  ExpressionParser expression_parser(this);
  branch_node->add_child(expression_parser.parse_statement(token));

  //Synchronize at the RIGHT_ARROW
  token = synchronize(SYM_SET);
  if (token->get_type() == (TokenType) PT_SYM)
  {
    token = next_token(token); //consume the RIGHT_ARROW
  }
  else {
    error_handler.flag(token, MISSING_SYM, this);
  }

  //parse the when_branch statement. The branch_node adopts
  //the statement subtree as its second child
  StatementParser statementParser(this);
  branch_node->add_child(statementParser.parse_statement(token));

  return branch_node;
}

}}}}  // namespace wci::frontend::pascal::parsers
