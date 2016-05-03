#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/MyASTUtil.h"
#include <iostream>

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace CAPA;

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
    const VarDecl        *mAcc;
    const VarDecl        *mAccRHS;

    ReduceInfo(const MatchFinder::MatchResult &Result)
    {
        mContext = Result.Context;
        mSM      = &Result.Context->getSourceManager();
        mLoop    = Result.Nodes.getNodeAs<ForStmt>("Reduce");
        mAssign  = Result.Nodes.getNodeAs<BinaryOperator>("Assign");
        mInitVar = Result.Nodes.getNodeAs<VarDecl>("InitVar");
        mIncVar  = Result.Nodes.getNodeAs<VarDecl>("IncVar");
        mInBase  = Result.Nodes.getNodeAs<VarDecl>("InBase");
        mInIndex = Result.Nodes.getNodeAs<VarDecl>("InIndex");
        mAcc     = Result.Nodes.getNodeAs<VarDecl>("Acc");
        mAccRHS  = Result.Nodes.getNodeAs<VarDecl>("AccRHS");
    }

    bool IsReduce()
    {
        std::cout << "Init: " << mInitVar << " Inc: " << mIncVar << " InIndex: " << mInIndex
                  << " Acc: " << mAcc << std::endl;
        if (mAccRHS)
        {
            return areSameVariable(2, mAccRHS, mAcc) && 
                   areSameVariable(3, mInitVar, mIncVar, mInIndex);
        }
        else
        {
            return areSameVariable(3, mInitVar, mIncVar, mInIndex);
        }
    }

    void ReduceDump()
    {
        std::cout << "This is a reduce" << std::endl;
        std::cout << node2str(mLoop, *mSM) << std::endl;
    }
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
        if (hi)
        {
            ReduceInfo r(result);
            if (r.IsReduce())
            {
                r.ReduceDump();
            }
        }
    }

    virtual void setUpMatcher() override
    {
        auto ReduceMatcher =
        forStmt(
            hasLoopInit(anyOf(
                declStmt(hasSingleDecl(varDecl(hasInitializer(
                    integerLiteral(anything()))).bind("InitVar"))),
                binaryOperator(
                    hasOperatorName("="),
                    hasLHS(declRefExpr(to(varDecl(hasType(
                        isInteger())).bind("InitVar"))))))),
            hasIncrement(unaryOperator(
                hasOperatorName("++"),
                hasUnaryOperand(declRefExpr(to(varDecl(hasType(isInteger())).bind("IncVar")))))),
            hasBody(anyOf(hasDescendant(
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
                    hasLHS(declRefExpr(to(varDecl().bind("Acc")))),
                    hasRHS(hasDescendant(arraySubscriptExpr(
                        hasBase(hasDescendant(declRefExpr(to(varDecl().bind("InBase"))))),
                        hasIndex(hasDescendant(declRefExpr(to(varDecl().bind("InIndex")))))))),
                        unless(hasDescendant(arraySubscriptExpr(hasDescendant(binaryOperator()))))
                    ).bind("Assign")),
                hasDescendant(binaryOperator(
                    hasOperatorName("="),
                    hasLHS(declRefExpr(to(varDecl().bind("Acc")))),
                    hasRHS(allOf(
                        hasDescendant(declRefExpr(to(varDecl().bind("AccRHS")))),
                        hasDescendant(arraySubscriptExpr(
                            hasIndex(hasDescendant(declRefExpr(to(
                                varDecl(hasType(isInteger())).bind("InIndex"))))),
                            hasBase(hasDescendant(declRefExpr(to(
                                varDecl().bind("InBase"))))))))),
                    unless(hasDescendant(arraySubscriptExpr(hasDescendant(binaryOperator()))))
            ).bind("Assign"))))).bind("Reduce");
        
        addMatcher(ReduceMatcher);

    }

};

static RuleSet rules(new ReduceRule());
