#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleSet.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class SwitchStatementsShouldHaveDefaultRule :
    public AbstractASTVisitorRule<SwitchStatementsShouldHaveDefaultRule>
{
public:
    virtual const string name() const override
    {
        return "switch statements should have default";
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
        if (switchStmt->isAllEnumCasesCovered())
        {
            return true;
        }

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
        if (!hasDefault)
        {
            addViolation(switchStmt, this);
        }

        return true;
    }
};

static RuleSet rules(new SwitchStatementsShouldHaveDefaultRule());
