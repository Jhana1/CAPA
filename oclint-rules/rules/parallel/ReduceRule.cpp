#include "oclint/AbstractASTMatcherRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/MyASTUtil.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace oclint;

class ReduceInfo
{
public:
    ASTContext           *mContext;
    SourceManager        *mSM;
    const ForStmt        *mLoop;
    const BinaryOperator *mAssign;
    const VarDecl        *mInitVar;
    const VarDecl        *mIncVar;
    const VarDecl        *mInBase;
    const VarDecl        *mInIndex;
    const VarDecl        *mOutBase;
    const VarDecl        *mOutIndex;
    const VarDecl        *mAcc;
};





class ReduceRule : public AbstractASTMatcherRule
{
protected:
    static ReduceInfo mInfo;
public:
    virtual const string name() const override
    {
        return "Reduce";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "Parallel";
    }

    virtual void callback(const MatchFinder::MatchResult &result) override
    {
        auto hi = result.Nodes.getNodeAs<ForStmt>("Reduce");
        hi->dumpColor();
    }

    virtual void setUpMatcher() override
    {
        auto ReduceMatcher =
        forStmt(
            hasLoopInit(anyOf(
                declStmt(hasSingleDecl(varDecl(hasInitializer(
                    integerLiteral(anything()))).bind("ReduceInitVar"))),
                binaryOperator(
                    hasOperatorName("="),
                    hasLHS(declRefExpr(to(varDecl(hasType(
                        isInteger())).bind("ReduceInitVar"))))))),
            hasIncrement(unaryOperator(
                hasOperatorName("++"),
                hasUnaryOperand(declRefExpr(to(varDecl(hasType(isInteger())).bind("ReduceIncVar")))))),
            hasBody(hasDescendant(anyOf(
                binaryOperator(anyOf(
                        hasOperatorName("+="),
                        hasOperatorName("-="),
                        hasOperatorName("*="),
                        hasOperatorName("/="),
                        hasOperatorName("%="),
                        hasOperatorName("<<="),
                        hasOperatorName(">>="),
                        hasOperatorName("&="),
                        hasOperatorName("^="),
                        hasOperatorName("|=")),
                    hasLHS(declRefExpr(to(varDecl().bind("ReduceAccLHS")))),
                    hasRHS(hasDescendant(arraySubscriptExpr(
                        hasBase(hasDescendant(declRefExpr(to(varDecl().bind("ReduceInBase"))))),
                        hasIndex(hasDescendant(declRefExpr(to(varDecl().bind("ReduceInIndex"))))))))
                    ).bind("ReduceAssignment"),
                binaryOperator(
                    hasOperatorName("="),
                    hasLHS(declRefExpr(to(varDecl().bind("ReduceAccLHS")))),
                    hasRHS(allOf(
                        hasDescendant(declRefExpr(to(varDecl().bind("ReduceAccRHS")))),
                        hasDescendant(arraySubscriptExpr(
                            hasIndex(hasDescendant(declRefExpr(to(
                                varDecl(hasType(isInteger())).bind("ReduceInIndex"))))),
                            hasBase(hasDescendant(declRefExpr(to(
                                varDecl().bind("ReduceInBase"))))))))
            )))))).bind("Reduce");
        
        addMatcher(ReduceMatcher);

    }

};

static RuleSet rules(new ReduceRule());
