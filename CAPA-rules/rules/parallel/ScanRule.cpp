#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/Util/MyASTUtil.h" 
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
    const VarDecl *mInBase;
    const VarDecl *mInIndex;
    const VarDecl *mAcc;
    const VarDecl *mAccRHS;

    ScanInfo(const MAtchFinder::MatchResult &Result)
    {
        mContext = Result.Context;
        mSM      = &Result.Context->getSourceManager();
        mLoop    = Result.Nodes.getNodeAs<ForStmt>("Scan");
        mAssign  = Result.Nodes.getNodeAs<BinaryOperator>("Assign");
        mInitVar = Result.Nodes.getNodeAs<VarDecl>("InitVar");
        mIncVar  = Result.Nodes.getNodeAs<VarDecl>("IncVar");
        mInBase  = Result.Nodes.getNodeAs<VarDecl>("InBase");                                        
        mInIndex = Result.Nodes.getNodeAs<VarDecl>("InIndex");                                       
        mAcc     = Result.Nodes.getNodeAs<VarDecl>("Acc");                                           
        mAccRHS  = Result.Nodes.getNodeAs<VarDecl>("AccRHS");                                        
    }                                                                                                
                                                                                                     
    bool IsScan()                                                                                  
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

class ScanRule : public AbstractASTMatcherRule
public:
    virtual const string name() const override
    {
        return "Scan";
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
    }

    virtual void setUpMatcher() override
    {

    }

};

static RuleSet rules(new ScanRule());
