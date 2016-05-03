#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleSet.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class CoveredSwitchStatementsDontNeedDefaultRule :
    public AbstractASTVisitorRule<CoveredSwitchStatementsDontNeedDefaultRule>
{
public:
    virtual const string name() const override
    {
        return "switch statements don't need default when fully covered";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "convention";
    }

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
        SwitchCase *currentSwitchCase = switchStmt->getSwitchCaseList();
        bool hasDefault = false;
        while (currentSwitchCase)
        {
            if (isa<DefaultStmt>(currentSwitchCase))
            {
                hasDefault = true;
                break;
            }
            currentSwitchCase = currentSwitchCase->getNextSwitchCase();
        }
        if (hasDefault && switchStmt->isAllEnumCasesCovered())
        {
            addViolation(switchStmt, this);
        }

        return true;
    }
};

static RuleSet rules(new CoveredSwitchStatementsDontNeedDefaultRule());
