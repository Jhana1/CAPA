#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleSet.h"

using namespace std;
using namespace clang;
using namespace CAPA;

class UselessParenthesesRule : public AbstractASTVisitorRule<UselessParenthesesRule>
{
private:
    void addParenExprToViolation(Expr *expr)
    {
        if (expr && isa<ParenExpr>(expr))
        {
            addViolation(expr, this);
        }
    }

public:
    virtual const string name() const override
    {
        return "useless parentheses";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "redundant";
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        addParenExprToViolation(ifStmt->getCond());

        return true;
    }

    bool VisitReturnStmt(ReturnStmt *returnStmt)
    {
        addParenExprToViolation(returnStmt->getRetValue());

        return true;
    }

    bool VisitVarDecl(VarDecl *varDecl)
    {
        addParenExprToViolation(varDecl->getInit());

        return true;
    }
};

static RuleSet rules(new UselessParenthesesRule());
