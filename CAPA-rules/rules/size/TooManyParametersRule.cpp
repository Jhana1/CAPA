#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleConfiguration.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class TooManyParametersRule : public AbstractASTVisitorRule<TooManyParametersRule>
{
private:
    unsigned int _threshold;

    template <typename T>
    void applyDecl(T *decl)
    {
        unsigned numOfParams = decl->param_size();
        if (decl->hasBody() && numOfParams > _threshold)
        {
            string description = "Method with " + toString<int>(numOfParams) +
                " parameters exceeds limit of " + toString<int>(_threshold);
            addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const override
    {
        return "too many parameters";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "size";
    }

    virtual void setUp() override
    {
        _threshold = RuleConfiguration::intForKey("TOO_MANY_PARAMETERS", 10);
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

static RuleSet rules(new TooManyParametersRule());
