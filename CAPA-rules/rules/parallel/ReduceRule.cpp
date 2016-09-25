#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/MyASTUtil.h"
#include "CAPA/helper/MatcherHelper.h"
#include <iostream>
#include <sstream>


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
    const Expr           *mCondRHS;
    const Expr           *mStride;
    const FunctionDecl   *mFunction;

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
        mCondRHS = Result.Nodes.getNodeAs<Expr>("ReduceCondRHS");
        mStride  = Result.Nodes.getNodeAs<Expr>("Stride");
        mFunction = Result.Nodes.getNodeAs<FunctionDecl>("Function");
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
        return areSameVariable(3, mInitVar, mIncVar, mInIndex);
    }


    int StrideSize()
    {
        if (mStride)
        {
            llvm::APSInt result;
            return (mStride->EvaluateAsInt(result, *mContext)) ? result.getExtValue() :  0;
        }
        return 1;
    }

    int Elements()
    {
        if (mCondRHS)
        {
            llvm::APSInt result;
            if (mCondRHS->EvaluateAsInt(result, *mContext))
            {
                return result.getExtValue();
            }
        }
        return 0;
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
        return 2;
    }

    virtual const string category() const override
    {
        return "Parallel";
    }

    virtual void callback(const MatchFinder::MatchResult &result) override
    {
        auto ReduceLoop = result.Nodes.getNodeAs<Stmt>("Reduce");
        if (ReduceLoop)
        {
            ReduceInfo r(result);
            if (markedIgnore(r.mFunction, *r.mSM))
                return;

            if (r.IsReduce())
            {
                //r.ReduceDump();
                PatternInfo p("Reduce", r.Elements(), r.sourceDump());
                std::stringstream extraInfo;
                int stride = r.StrideSize();
                int elements = r.Elements();

                if (stride == 0)
                    extraInfo << "Stride Size: " << "Unknown. ";
                else
                    extraInfo << "Stride Size: " << stride << ". ";

                if (elements == 0)
                    extraInfo << "Number of Elements: " << "Unknown. ";
                else
                    extraInfo << "Number of Elements: " << r.Elements() << ". ";

                addViolation(ReduceLoop, this, p, extraInfo.str());
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

        auto ForStmtReduceMatcher = FunctionWrap(ForLoop("Reduce", "", body));
        auto WhileStmtReduceMatcher = FunctionWrap(WhileLoop("Reduce", "", body));
       
        addMatcher(ForStmtReduceMatcher);
        addMatcher(WhileStmtReduceMatcher);
    }

};

static RuleSet rules(new ReduceRule());
