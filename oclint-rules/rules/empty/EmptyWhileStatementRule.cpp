#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class EmptyWhileStatementRule : public AbstractEmptyBlockStmtRule<EmptyWhileStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty while statement";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "empty";
    }

    bool VisitWhileStmt(WhileStmt *whileStmt)
    {
        return checkLexicalEmptyStmt(whileStmt->getBody(), this);
    }
};

static RuleSet rules(new EmptyWhileStatementRule());
