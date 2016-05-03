#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleConfiguration.h"
#include "CAPA/RuleSet.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class TooFewBranchesInSwitchStatementRule :
    public AbstractASTVisitorRule<TooFewBranchesInSwitchStatementRule>
{
    class CountCaseStmts : public RecursiveASTVisitor<CountCaseStmts>
    {
    private:
        int _count;

    public:
        int count(SwitchStmt *switchStmt)
        {
            _count = 0;
            (void) /* explicitly ignore the return of this function */ TraverseStmt(switchStmt);
            return _count;
        }

        bool VisitCaseStmt(CaseStmt *)
        {
            _count++;
            return true;
        }
    };

public:
    virtual const string name() const override
    {
        return "too few branches in switch statement";
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
        CountCaseStmts countCaseStmts;
        int numberOfCaseStmts = countCaseStmts.count(switchStmt);
        int threshold = RuleConfiguration::intForKey("MINIMUM_CASES_IN_SWITCH", 3);
        if (numberOfCaseStmts < threshold)
        {
            addViolation(switchStmt, this);
        }

        return true;
    }
};

static RuleSet rules(new TooFewBranchesInSwitchStatementRule());
