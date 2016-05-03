#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleConfiguration.h"
#include "CAPA/RuleSet.h"
#include "CAPA/metric/NcssMetric.h"
#include "CAPA/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class NcssMethodCountRule : public AbstractASTVisitorRule<NcssMethodCountRule>
{
private:
    void applyDecl(Decl *decl)
    {
        int ncss = getNcssCount(decl);
        int threshold = RuleConfiguration::intForKey("NCSS_METHOD", 30);
        if (ncss > threshold)
        {
            string description = "Method of " + toString<int>(ncss) +
                " non-commenting source statements exceeds limit of " + toString<int>(threshold);
            addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const override
    {
        return "high ncss method";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "size";
    }

    bool VisitObjCMethodDecl(ObjCMethodDecl *decl)
    {
        applyDecl(decl);
        return true;
    }

    bool VisitFunctionDecl(FunctionDecl *decl)
    {
        applyDecl(decl);
        return true;
    }
};

static RuleSet rules(new NcssMethodCountRule());
