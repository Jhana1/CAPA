#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/MyASTUtil.h"
#include "CAPA/helper/MatcherHelper.h"
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
    const Stmt           *mLoop;
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
        mLoop    = Result.Nodes.getNodeAs<Stmt>("Reduce");
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
        //std::cout << "Init: " << mInitVar << " Inc: " << mIncVar << " InIndex: " << mInIndex
        //          << " Acc: " << mAcc << std::endl;
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

    std::string sourceDump()
    {
        return node2str(mLoop, *mSM);
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
        std::cout << "Calledback" << std::endl;
        auto ReduceLoop = result.Nodes.getNodeAs<Stmt>("Reduce");
        if (ReduceLoop)
        {
            ReduceInfo r(result);
            if (r.IsReduce())
            {
                //r.ReduceDump();
                PatternInfo p("Reduce", r.sourceDump());
                addViolation(ReduceLoop, this, p, "A Reduction");
            }
        }
    }

    virtual void setUpMatcher() override
    {
        auto left = VarBind("Acc");
        auto right1 = hasDescendant(VectorBind("In"));
        auto right2 = allOf(DVarBind("AccRHS"),
                            hasDescendant(VectorBind("In")));
        
        auto body = anyOf(hasDescendant(BinaryOperatorBindReduceAll("Assign", left, right1)),
                          hasDescendant(BinaryOperatorBindReduce("Assign", left, right2)));

        auto ForStmtReduceMatcher = ForLoop("Reduce", "", body);
        auto WhileStmtReduceMatcher = WhileLoop("Reduce", "", body);
        
        addMatcher(ForStmtReduceMatcher);
        addMatcher(WhileStmtReduceMatcher);
    }

};

static RuleSet rules(new ReduceRule());
