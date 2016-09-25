#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/MyASTUtil.h"
#include "CAPA/helper/MatcherHelper.h"
#include <iostream>
#include <map>
#include <vector>
#include <sstream> 

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace CAPA;

class MapInfo
{
public:
    ASTContext            *mContext;
    SourceManager         *mSM;
    const Stmt            *mLoop;
    const BinaryOperator  *mAssign;
    const VarDecl         *mInitVar;
    const VarDecl         *mIncVar;
    const Expr            *mStride;
    const VarDecl         *mInBase;
    const VarDecl         *mInIndex;
    const VarDecl         *mOutBase;
    const VarDecl         *mOutIndex;
    const Expr            *mCondRHS;
    const FunctionDecl    *mFunction;

    MapInfo(const MatchFinder::MatchResult &Result)
    {
        mContext     = Result.Context;
        mSM          = &Result.Context->getSourceManager();
        mLoop        = Result.Nodes.getNodeAs<Stmt>("Map");
        mAssign      = Result.Nodes.getNodeAs<BinaryOperator>("Assign");
        mInitVar     = Result.Nodes.getNodeAs<VarDecl>("InitVar");
        mIncVar      = Result.Nodes.getNodeAs<VarDecl>("IncVar");
        mStride      = Result.Nodes.getNodeAs<Expr>("Stride");
        mInBase      = Result.Nodes.getNodeAs<VarDecl>("InBase");
        mInIndex     = Result.Nodes.getNodeAs<VarDecl>("InIndex");
        mOutBase     = Result.Nodes.getNodeAs<VarDecl>("OutBase");
        mOutIndex    = Result.Nodes.getNodeAs<VarDecl>("OutIndex");
        mCondRHS     = Result.Nodes.getNodeAs<Expr>("MapCondRHS");
        mFunction    = Result.Nodes.getNodeAs<FunctionDecl>("Function");
    }
    
    bool IsMap()
    {
//        std::cout << "Init: " << mInitVar << " Inc: " << mIncVar << " InIndex: " << mInIndex
//                  << " OutIndex: " << mOutIndex << std::endl;
        return areSameVariable(2, mIncVar, mOutIndex);
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
            if (mCondRHS->EvaluateAsInt(result, *mContext))
            {
                return result.getExtValue();
            }
        }
        return 0;
    }

    std::string MapDump()
    {
//        std::cout << "This is a Map" << std::endl;
        return node2str(mLoop, *mSM);
    }
};



class MapRule : public AbstractASTMatcherRule
{

protected:
    static std::map<const Stmt*, MapInfo> mMapStatus;

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
        auto MapLoop = Result.Nodes.getStmtAs<Stmt>("Map");
        
        // Check if the pointer is null, if it is then we don't have a map.
        if (MapLoop)
        {
            MapInfo currentMap(Result);
            // Check if marked return by comments
            if (markedIgnore(currentMap.mFunction, *currentMap.mSM))
                return;

            // Check if we've had this map before
            // If we have then it has had 2 callbacks, making it almost certainly not a map
            if (mMapStatus.find(currentMap.mLoop) != mMapStatus.end())
            {
                removeViolation(MapLoop, this);
                return;
            }

            if (currentMap.IsMap())
            {
                PatternInfo p("Map", currentMap.Elements(), currentMap.MapDump());
                std::stringstream extraInfo; 
                auto stride = currentMap.StrideSize();
                if (stride == 0)
                    extraInfo << "Stride Size: " << "Unknown. ";
                else
                    extraInfo << "Stride Size: " << stride << ". ";
                
                if (currentMap.Elements() == 0)
                    extraInfo << "Number of Elements: " << "Unknown. ";
                else
                    extraInfo << "Number of Elements: " << currentMap.Elements() << ". ";

                addViolation(MapLoop, this, p, extraInfo.str());
            }
        }
    }

    virtual void setUpMatcher() override
    {
        // Matches on For Loops with counter initialised in the init, with an array element
        // assignment within the body of the loop
        auto left = VectorBind("Out");
        auto right = hasDescendant(VectorBind("In"));
        auto unless = hasDescendant(arraySubscriptExpr(hasDescendant(binaryOperator())));

        auto body = hasDescendant(BinaryOperatorBindUnless("=", "Assign", left, right, unless)); 
        auto body2 = hasDescendant(BinaryOperatorBindAll("Assign", left, 
                         NumericLiteralBind("Literal")));

        auto ForMatcher = FunctionWrap(ForLoop("Map", "", anyOf(body, body2))); 

        auto WhileMatcher = FunctionWrap(WhileLoop("Map", "", body));

        
        addMatcher(ForMatcher);
        addMatcher(WhileMatcher);
    }

};

static RuleSet rules(new MapRule());
std::map<const Stmt*, MapInfo> MapRule::mMapStatus;
