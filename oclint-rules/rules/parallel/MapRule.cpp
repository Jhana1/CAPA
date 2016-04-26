#include "oclint/AbstractASTMatcherRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/MyASTUtil.h"
#include <iostream>

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace oclint;

class MapInfo
{
public:
    ASTContext            *mContext;
    SourceManager         *mSM;
    const ForStmt         *mLoop;
    const BinaryOperator  *mAssign;
    const VarDecl         *mInitVar;
    const VarDecl         *mIncVar;
    const VarDecl         *mInBase;
    const VarDecl         *mInIndex;
    const VarDecl         *mOutBase;
    const VarDecl         *mOutIndex;
    
    MapInfo(const MatchFinder::MatchResult &Result)
    {
        mContext     = Result.Context;
        mSM          = &Result.Context->getSourceManager();
        mLoop        = Result.Nodes.getNodeAs<ForStmt>("MapLoop");
        mAssign      = Result.Nodes.getNodeAs<BinaryOperator>("MapAssign");
        mInitVar     = Result.Nodes.getNodeAs<VarDecl>("MapInitVar");
        mIncVar      = Result.Nodes.getNodeAs<VarDecl>("MapIncVar");
        mInBase      = Result.Nodes.getNodeAs<VarDecl>("MapInBase");
        mInIndex     = Result.Nodes.getNodeAs<VarDecl>("MapInIndex");
        mOutBase     = Result.Nodes.getNodeAs<VarDecl>("MapOutBase");
        mOutIndex    = Result.Nodes.getNodeAs<VarDecl>("MapOutIndex");
    }
    
    bool IsMap()
    {
        std::cout << "Init: " << mInitVar << " Inc: " << mIncVar << " InIndex: " << mInIndex
                  << " OutIndex: " << mOutIndex << std::endl;
        return areSameVariable(4, mInitVar, mIncVar, mInIndex, mOutIndex);
    }

    void MapDump()
    {
        std::cout << "This is a Map" << std::endl;
        std::cout << node2str(mLoop, *mSM) << std::endl;
    }
};



class MapRule : public AbstractASTMatcherRule
{

protected:
    static std::vector<MapInfo> mInfo;
    static std::map<const ForStmt*, bool> mMapStatus;

public:
    virtual const string name() const override
    {
        return "Nothing";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "custom";
    }

    virtual void callback(const MatchFinder::MatchResult &Result) override
    {
        auto Context = Result.Context;
        auto MapLoop = Result.Nodes.getStmtAs<ForStmt>("MapLoop");
        
        std::cout << MapLoop << std::endl;
        if (MapLoop)
        {
            MapInfo m(Result);
            if (m.IsMap())
            {
                m.MapDump();
            }
            
        }

    }

    virtual void setUpMatcher() override
    {
        // Matches on For Loops with counter initialised in the init, with an array element
        // assignment within the body of the loop
        auto MapMatcher =
        forStmt(
            hasLoopInit(anyOf(
                declStmt(hasSingleDecl(varDecl(hasInitializer(
                    integerLiteral(anything()))).bind("MapInitVar"))),
                binaryOperator(
                    hasOperatorName("="),
                    hasLHS(declRefExpr(to(varDecl(hasType(
                        isInteger())).bind("MapInitVar"))))))),
            hasIncrement(unaryOperator(
                hasOperatorName("++"),
                hasUnaryOperand(declRefExpr(to(varDecl(hasType(isInteger())).bind("MapIncVar")))))),
            hasBody(has(binaryOperator(
                hasOperatorName("="),
                hasLHS(arraySubscriptExpr(
                    hasBase(implicitCastExpr(hasSourceExpression(declRefExpr(to(
                        varDecl().bind("MapOutBase")))))),
                    hasIndex(hasDescendant(declRefExpr(to(varDecl(hasType(
                        isInteger())).bind("MapOutIndex"))))))),
                hasRHS(forEachDescendant(arraySubscriptExpr(hasBase(implicitCastExpr(
                        hasSourceExpression(declRefExpr(to(varDecl().bind("MapInBase")))))),
                            hasIndex(hasDescendant(declRefExpr(to(varDecl(hasType(
                                isInteger())).bind("MapInIndex")))))))),
                unless(hasDescendant(arraySubscriptExpr(hasDescendant(binaryOperator())))))
                .bind("MapAssign")))).bind("MapLoop");


        addMatcher(MapMatcher);
    }

};

static RuleSet rules(new MapRule());
