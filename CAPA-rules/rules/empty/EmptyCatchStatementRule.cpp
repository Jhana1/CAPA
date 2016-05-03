#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class EmptyCatchStatementRule : public AbstractEmptyBlockStmtRule<EmptyCatchStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty catch statement";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "empty";
    }

    bool VisitCXXCatchStmt(CXXCatchStmt *catchStmt)
    {
        return checkLexicalEmptyStmt(catchStmt->getHandlerBlock(), this);
    }

    bool VisitObjCAtCatchStmt(ObjCAtCatchStmt *catchStmt)
    {
        return checkLexicalEmptyStmt(catchStmt->getCatchBody(), this);
    }
};

static RuleSet rules(new EmptyCatchStatementRule());
