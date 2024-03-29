/*
 * WhenExecutor.h
 *
 *  Created on: Feb 27, 2018
 *      Author: shervin

 * <h1>WhenExecutor</h1>
 *
 * <p>Execute an WHEN statement.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef WHENEXECUTOR_H_
#define WHENEXECUTOR_H_

#include "StatementExecutor.h"
#include "../../../Object.h"
#include "../../../intermediate/ICodeNode.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;

class WhenExecutor : public StatementExecutor
{
public:
    /**
     * Constructor.
     * @param the parent executor.
     */
    WhenExecutor(Executor *parent);

    /**
     * Execute a compound statement.
     * @param node the root node of the compound statement.
     * @return nullptr.
     */
    Object execute(ICodeNode *node);
};

}}}}  // namespace wci::backend::interpreter::executors

#endif /* WHENEXECUTOR_H_ */
