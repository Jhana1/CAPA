#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class EmptyForStatementRule : public AbstractEmptyBlockStmtRule<EmptyForStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty for statement";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "empty";
    }

    bool VisitForStmt(ForStmt *forStmt)
    {
        return checkLexicalEmptyStmt(forStmt->getBody(), this);
    }

    bool VisitObjCForCollectionStmt(ObjCForCollectionStmt *forStmt)
    {
        return checkLexicalEmptyStmt(forStmt->getBody(), this);
    }
};

static RuleSet rules(new EmptyForStatementRule());
