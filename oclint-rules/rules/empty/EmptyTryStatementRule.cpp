#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class EmptyTryStatementRule : public AbstractEmptyBlockStmtRule<EmptyTryStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty try statement";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "empty";
    }

    bool VisitCXXTryStmt(CXXTryStmt *tryStmt)
    {
        return checkLexicalEmptyStmt(tryStmt->getTryBlock(), this);
    }

    bool VisitObjCAtTryStmt(ObjCAtTryStmt *tryStmt)
    {
        return checkLexicalEmptyStmt(tryStmt->getTryBody(), this);
    }
};

static RuleSet rules(new EmptyTryStatementRule());
