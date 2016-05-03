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
        mLoop        = Result.Nodes.getNodeAs<ForStmt>("Map");
        mAssign      = Result.Nodes.getNodeAs<BinaryOperator>("Assign");
        mInitVar     = Result.Nodes.getNodeAs<VarDecl>("InitVar");
        mIncVar      = Result.Nodes.getNodeAs<VarDecl>("IncVar");
        mInBase      = Result.Nodes.getNodeAs<VarDecl>("InBase");
        mInIndex     = Result.Nodes.getNodeAs<VarDecl>("InIndex");
        mOutBase     = Result.Nodes.getNodeAs<VarDecl>("OutBase");
        mOutIndex    = Result.Nodes.getNodeAs<VarDecl>("OutIndex");
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
        return "Map";
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
        auto MapLoop = Result.Nodes.getStmtAs<ForStmt>("Map");
        
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
                    integerLiteral(anything()))).bind("InitVar"))),
                binaryOperator(
                    hasOperatorName("="),
                    hasLHS(declRefExpr(to(varDecl(hasType(
                        isInteger())).bind("InitVar"))))))),
            hasIncrement(unaryOperator(
                hasOperatorName("++"),
                hasUnaryOperand(declRefExpr(to(varDecl(hasType(isInteger())).bind("IncVar")))))),
            hasBody(hasDescendant(binaryOperator(
                hasOperatorName("="),
                hasLHS(arraySubscriptExpr(
                    hasBase(implicitCastExpr(hasSourceExpression(declRefExpr(to(
                        varDecl().bind("OutBase")))))),
                    hasIndex(hasDescendant(declRefExpr(to(varDecl(hasType(
                        isInteger())).bind("OutIndex"))))))),
                hasRHS(hasDescendant(arraySubscriptExpr(hasBase(implicitCastExpr(
                        hasSourceExpression(declRefExpr(to(varDecl().bind("InBase")))))),
                            hasIndex(hasDescendant(declRefExpr(to(varDecl(hasType(
                                isInteger())).bind("InIndex")))))))),
                unless(hasDescendant(arraySubscriptExpr(hasDescendant(binaryOperator())))))
                .bind("Assign")))).bind("Map");


        addMatcher(MapMatcher);
    }

};

static RuleSet rules(new MapRule());
