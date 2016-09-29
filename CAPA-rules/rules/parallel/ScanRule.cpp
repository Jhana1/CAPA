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

class ScanInfo
{
public:
    ASTContext *mContext;
    SourceManager *mSM;
    const ForStmt *mLoop;
    const BinaryOperator *mAssign;
    const VarDecl *mInitVar;
    const VarDecl *mIncVar;
    const VarDecl *mInBase1;
    const VarDecl *mInIndex1;
    const VarDecl *mInIndexBO;
    const VarDecl *mOutBase;
    const VarDecl *mOutIndex;
    const Expr    *mStride;
    const Expr    *mCondRHS;
    const FunctionDecl *mFunction;

    ScanInfo(const MatchFinder::MatchResult &Result)
    {
        mContext   = Result.Context;
        mSM        = &Result.Context->getSourceManager();
        mLoop      = Result.Nodes.getNodeAs<ForStmt>("Scan");
        mAssign    = Result.Nodes.getNodeAs<BinaryOperator>("Assign");
        mInitVar   = Result.Nodes.getNodeAs<VarDecl>("InitVar");
        mIncVar    = Result.Nodes.getNodeAs<VarDecl>("IncVar");
        mInBase1   = Result.Nodes.getNodeAs<VarDecl>("InBase");                                        
        mInIndex1  = Result.Nodes.getNodeAs<VarDecl>("InIndex");
        mInIndexBO = Result.Nodes.getNodeAs<VarDecl>("InIndexBO");
        mOutBase   = Result.Nodes.getNodeAs<VarDecl>("OutBase");
        mOutIndex  = Result.Nodes.getNodeAs<VarDecl>("OutIndex");
        mStride    = Result.Nodes.getNodeAs<Expr>("Stride");
        mCondRHS   = Result.Nodes.getNodeAs<Expr>("ScanCondRHS");
        mFunction  = Result.Nodes.getNodeAs<FunctionDecl>("Function");
    }                                                                                                
                                                                                                     
    bool IsScan()                                                                                  
    {                                                                                                
        //std::cout << "Init: " << mInitVar << " Inc: " << mIncVar << " InIndex: " << mInIndex       
        //          << " Acc: " << mAcc << std::endl;                                                
        return (areSameVariable(4, mInitVar, mIncVar, mInIndex1, mOutIndex) ||
               areSameVariable(4, mInitVar, mIncVar, mInIndexBO, mOutIndex)) &&
               mInIndexBO;                                  
    }                                                                                                

    int StrideSize()
    {
        if (mStride)
        {
            llvm::APSInt result;
            return (mStride->EvaluateAsInt(result, *mContext)) ? result.getExtValue() : 0;
        }
        return 1;
    }

    int Elements()
    {
        if (mCondRHS)
        {
            llvm::APSInt result;
            return (mCondRHS->EvaluateAsInt(result, *mContext)) ? result.getExtValue() : 0;
        }
        return 0;
    }

    std::string sourceDump()                                                                         
    {                                                                                                
        return node2str(mLoop, *mSM);                                                                
    }                                                                                                
};            

class ScanRule : public AbstractASTMatcherRule
{
public:
    virtual const string name() const override
    {
        return "Scan";
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
        auto ScanLoop = result.Nodes.getNodeAs<Stmt>("Scan");
        if (ScanLoop)
        {
            ScanInfo r(result);
            if (markedIgnore(r.mFunction, *r.mSM))
                return;

            if (r.IsScan())
            {
                PatternInfo p("Scan", r.Elements(), r.sourceDump());
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
                    extraInfo << "Number of Elements: " << elements << ". ";

                addViolation(ScanLoop, this, p, extraInfo.str());
            }
        }
    }

    virtual void setUpMatcher() override
    {

        auto left = VectorBind("Out");
        auto right = forEachDescendant(VectorBindScan("In"));
        auto body = anyOf(HasDescendant(BinaryOperatorBind("=", "Assign", left, right)),
                          HasDescendant(BinaryOperatorBindAll("Assign",  left, right))); 

        auto ScanMatcher = FunctionWrap(ForLoopNoParentLoop("Scan", "", body));

        addMatcher(ScanMatcher);
    }
};


static RuleSet rules(new ScanRule());
