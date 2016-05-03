#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleConfiguration.h"
#include "CAPA/RuleSet.h"
#include "CAPA/metric/StmtDepthMetric.h"
#include "CAPA/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class NestedBlockDepthRule : public AbstractASTVisitorRule<NestedBlockDepthRule>
{
public:
    virtual const string name() const override
    {
        return "deep nested block";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "size";
    }

    bool VisitCompoundStmt(CompoundStmt *compoundStmt)
    {
        int depth = getStmtDepth(compoundStmt);
        int threshold = RuleConfiguration::intForKey("NESTED_BLOCK_DEPTH", 5);
        if (depth > threshold)
        {
            string description = "Block depth of " + toString<int>(depth) +
                " exceeds limit of " + toString<int>(threshold);
            addViolation(compoundStmt, this, description);
        }

        return true;
    }
};

static RuleSet rules(new NestedBlockDepthRule());
