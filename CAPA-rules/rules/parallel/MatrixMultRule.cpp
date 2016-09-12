#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/MyASTUtil.h"
#include <iostream>

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace CAPA;

class MatrixMultInfo
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

    MatrixMultInfo(const MatchFinder::MatchResult &Result)
    {
        mContext = Result.Context;
        mSM      = &Result.Context->getSourceManager();
        mLoop    = Result.Nodes.getNodeAs<Stmt>("MatrixMult");
        mInitVar = Result.Nodes.getNodeAs<VarDecl>("InitVar");
        mIncVar  = Result.Nodes.getNodeAs<VarDecl>("IncVar");
        mInBase  = Result.Nodes.getNodeAs<VarDecl>("InBase");
        mInIndex = Result.Nodes.getNodeAs<VarDecl>("InIndex");
        mAcc     = Result.Nodes.getNodeAs<VarDecl>("Acc");
        mAccRHS  = Result.Nodes.getNodeAs<VarDecl>("AccRHS");
    }

    bool IsMatrixMult()
    {
        return true;
    }

    std::string sourceDump()
    {
        return node2str(mLoop, *mSM);
    }
};





class MatrixMultRule : public AbstractASTMatcherRule
{
protected:
    static MatrixMultInfo mInfo;
public:
    virtual const string name() const override
    {
        return "Matrix Multiplication";
    }

    virtual int priority() const override
    {
        return 1;
    }

    virtual const string category() const override
    {
        return "Parallel";
    }

    virtual void callback(const MatchFinder::MatchResult &result) override
    {
        auto OuterLoop = result.Nodes.getNodeAs<Stmt>("MatrixMult");
        if (OuterLoop)
        {
            MatrixMultInfo r(result);
            if (r.IsMatrixMult())
            {
                PatternInfo p("MatrixMult", 124, r.sourceDump());
                addViolation(OuterLoop, this, p, "A Matrix Multiply");
            }
        }
    }

    virtual void setUpMatcher() override
    {
        auto VarB = [&](std::string binding)
        {
            return declRefExpr(to(varDecl().bind(binding)));
        };

        auto dVarB = [&](std::string binding)
        {
            return hasDescendant(VarB(binding));
        };
        
        auto LoopInit = [&](std::string level)
        {
            return anyOf(
                declStmt(hasSingleDecl(varDecl(hasInitializer(
                    integerLiteral(anything()))).bind("InitVar" + level))),
                binaryOperator(
                    hasOperatorName("="),
                    hasLHS(VarB("InitVar" + level))));
        };

        auto LoopIncrement = [&](std::string level) 
        {
            return unaryOperator(
                hasOperatorName("++"),
                hasUnaryOperand(VarB("IncVar" + level)));
        };

        auto ForLoop = [&](std::string bindingLevel, auto injectBody)
        {
            return forStmt(
                    hasLoopInit(LoopInit(bindingLevel)),
                    hasIncrement(LoopIncrement(bindingLevel)),
                    hasBody(injectBody));
        };

        auto MatrixBind = [&](std::string binding)
        {
            return arraySubscriptExpr(
                    hasBase(hasDescendant(arraySubscriptExpr(
                        hasBase(dVarB(binding + "Base")),
                        hasIndex(dVarB(binding + "Row"))))),
                    hasIndex(dVarB(binding + "Column")));
        };

        auto LoopBody = 
            hasDescendant(
                binaryOperator(
                    hasOperatorName("+="),
                    hasLHS(MatrixBind("Out")),
                    hasRHS(binaryOperator(
                        hasOperatorName("*"),
                        hasLHS(hasDescendant(MatrixBind("Left"))),
                        hasRHS(hasDescendant(MatrixBind("Right"))))
            )));

        auto MatrixMultMatcher = 
            ForLoop("0", hasDescendant(
                ForLoop("1",hasDescendant(
                    ForLoop("2", LoopBody))))).bind("MatrixMult");

        addMatcher(MatrixMultMatcher);

    }

};

static RuleSet rules(new MatrixMultRule());
