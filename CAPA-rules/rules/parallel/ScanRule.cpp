#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/MyASTUtil.h" 
#include "CAPA/helper/MatcherHelper.h"
#include <iostream>

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

    ScanInfo(const MatchFinder::MatchResult &Result)
    {
        mContext   = Result.Context;
        mSM        = &Result.Context->getSourceManager();
        mLoop      = Result.Nodes.getNodeAs<ForStmt>("ScanLoop");
        mAssign    = Result.Nodes.getNodeAs<BinaryOperator>("Assign");
        mInitVar   = Result.Nodes.getNodeAs<VarDecl>("InitVar");
        mIncVar    = Result.Nodes.getNodeAs<VarDecl>("IncVar");
        mInBase1   = Result.Nodes.getNodeAs<VarDecl>("InBase");                                        
        mInIndex1  = Result.Nodes.getNodeAs<VarDecl>("InIndex");
        mInIndexBO = Result.Nodes.getNodeAs<VarDecl>("InIndexBO");
        mOutBase   = Result.Nodes.getNodeAs<VarDecl>("OutBase");
        mOutIndex  = Result.Nodes.getNodeAs<VarDecl>("OutIndex");
    }                                                                                                
                                                                                                     
    bool IsScan()                                                                                  
    {                                                                                                
        //std::cout << "Init: " << mInitVar << " Inc: " << mIncVar << " InIndex: " << mInIndex       
        //          << " Acc: " << mAcc << std::endl;                                                
        return //areSameVariable(4, mInitVar, mIncVar, mInIndex1, mOutIndex) ||
               areSameVariable(4, mInitVar, mIncVar, mInIndexBO, mOutIndex);                                  
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
        auto ScanLoop = result.Nodes.getNodeAs<Stmt>("ScanLoop");
        if (ScanLoop)
        {
            ScanInfo r(result);
            if (r.IsScan())
            {
                PatternInfo p("Scan", r.sourceDump());
                addViolation(ScanLoop, this, p, "A Scan");
            }
        }
    }

    virtual void setUpMatcher() override
    {

        auto left = VectorBind("Out");
        auto right = forEachDescendant(VectorBindScan("In"));
        auto body = anyOf(hasDescendant(BinaryOperatorBind("=", "Assign", left, right)),
                          hasDescendant(BinaryOperatorBindAll("Assign",  left, right))); 

        auto ScanMatcher = ForLoop("ScanLoop", "", body);

        addMatcher(ScanMatcher);
    }
};


static RuleSet rules(new ScanRule());
