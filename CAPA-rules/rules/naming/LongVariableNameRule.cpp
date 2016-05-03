#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleConfiguration.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class LongVariableNameRule : public AbstractASTVisitorRule<LongVariableNameRule>
{
public:
    virtual const string name() const override
    {
        return "long variable name";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "naming";
    }

    bool VisitVarDecl(VarDecl *varDecl)
    {
        int nameLength = varDecl->getNameAsString().size();
        int threshold = RuleConfiguration::intForKey("LONG_VARIABLE_NAME", 20);
        if (nameLength > threshold)
        {
            string description = "Variable name with " + toString<int>(nameLength) +
                " characters is longer than the threshold of " + toString<int>(threshold);
            addViolation(varDecl, this, description);
        }

        return true;
    }
};

static RuleSet rules(new LongVariableNameRule());
