#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleSet.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class ConstantIfExpressionRule : public AbstractASTVisitorRule<ConstantIfExpressionRule>
{
public:
    virtual const string name() const override
    {
        return "constant if expression";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "basic";
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        Expr *conditionExpr = ifStmt->getCond();

        bool evaluatedResult;
        if (conditionExpr->EvaluateAsBooleanCondition(evaluatedResult, *_carrier->getASTContext()))
        {
            addViolation(conditionExpr, this);
        }

        return true;
    }
};

static RuleSet rules(new ConstantIfExpressionRule());
