#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/MyASTUtil.h" 
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
        return 3;
    }

    virtual const string category() const override
    {
        return "Parallel";
    }

    virtual void callback(const MatchFinder::MatchResult &result) override
    {
        auto ScanLoop = result.Nodes.getNodeAs<ForStmt>("ScanLoop");
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
        auto ScanMatcher = 
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
            hasBody(anyOf(
                hasDescendant(binaryOperator(
                    hasOperatorName("="),
                    hasLHS(arraySubscriptExpr(
                        hasBase(hasDescendant(declRefExpr(to(varDecl().bind("OutBase"))))),
                        hasIndex(hasDescendant(declRefExpr(to(varDecl(hasType(
                            isInteger())).bind("OutIndex"))))))),
                    hasRHS(forEachDescendant(// TODO: Make this anyOf and include other TODO's
                        // Variable Assigned to itself + some previous of itself
                        // Or assigned as a monoid concat of 2 elements from a different array
                        arraySubscriptExpr(
                                hasBase(hasDescendant(declRefExpr(to(varDecl().bind("InBase"))))),
                                hasIndex(anyOf(
                                    binaryOperator(forEachDescendant(declRefExpr(to(
                                        varDecl().bind("InIndexBO"))))),
                                    hasDescendant(declRefExpr(to(varDecl().bind("InIndex"))))))
                            )))).bind("Assign")),

               hasDescendant(binaryOperator(anyOf(
                        hasOperatorName("+="),
                        hasOperatorName("-="),
                        hasOperatorName("/="),
                        hasOperatorName("*="),
                        hasOperatorName("<<="),
                        hasOperatorName(">>="),
                        hasOperatorName("|="),
                        hasOperatorName("&="),
                        hasOperatorName("%="),
                        hasOperatorName("^=")),
                    hasLHS(arraySubscriptExpr(
                        hasBase(hasDescendant(declRefExpr(to(varDecl().bind("OutBase"))))),
                        hasIndex(hasDescendant(declRefExpr(to(varDecl().bind("OutIndex"))))))),
                    hasRHS(forEachDescendant(
                        arraySubscriptExpr(hasBase(hasDescendant(declRefExpr(to(
                            varDecl().bind("InBase"))))),
                        hasIndex(binaryOperator(forEachDescendant(declRefExpr(to(
                            varDecl().bind("InIndexBO")))))))))).bind("Assign")) 
                ))).bind("ScanLoop");
                            /*hasLHS(hasDescendant(arraySubscriptExpr(
                                hasBase(hasDescendant(declRefExpr(to(varDecl().bind("InBase1"))))),
                                hasIndex(hasDescendant(declRefExpr(to(varDecl(hasType(
                                    isInteger())).bind("InIndex1")))))))),
                            hasRHS(hasDescendant(arraySubscriptExpr(
                                hasBase(hasDescendant(declRefExpr(to(varDecl().bind("InBase2"))))),
                                hasIndex(hasDescendant(declRefExpr(to(varDecl(hasType(
                                    isInteger())).bind("InIndex2"))))))))*/
                        // TODO: Variable assigned to itself + some previous via function application
                /*    ))))),
                hasDescendant(binaryOperator(anyOf(
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
                    hasLHS(arraySubscriptExpr(
                        hasBase(hasDescendant(declRefExpr(to(varDecl().bind("OutBase"))))),
                        hasIndex(hasDescendant(declRefExpr(to(varDecl(hasType(
                            isInteger())).bind("OutIndex"))))))),
                    hasRHS(hasDescendant(arraySubscriptExpr(
                            hasBase(hasDescendant(declRefExpr(to(varDecl().bind("InBase1"))))),
                            hasIndex(hasDescendant(declRefExpr(to(varDecl().bind("InIndex1"))))))))))
                ))).bind("ScanLoop");*/
        
        addMatcher(ScanMatcher);
    }
};


static RuleSet rules(new ScanRule());
