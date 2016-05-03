#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleSet.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class ConstantConditionalOperatorRule :
    public AbstractASTVisitorRule<ConstantConditionalOperatorRule>
{
public:
    virtual const string name() const override
    {
        return "constant conditional operator";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "basic";
    }

    bool VisitConditionalOperator(ConditionalOperator *conditionalOperator)
    {
        Expr *conditionExpr = conditionalOperator->getCond();

        bool evaluatedResult;
        if (conditionExpr->EvaluateAsBooleanCondition(evaluatedResult, *_carrier->getASTContext()))
        {
            addViolation(conditionExpr, this);
        }

        return true;
    }
};

static RuleSet rules(new ConstantConditionalOperatorRule());
