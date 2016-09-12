#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/MyASTUtil.h"
#include "CAPA/helper/MatcherHelper.h"
#include <iostream>
#include <map>
#include <vector>

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
    }
    
    bool IsMap()
    {
//        std::cout << "Init: " << mInitVar << " Inc: " << mIncVar << " InIndex: " << mInIndex
//                  << " OutIndex: " << mOutIndex << std::endl;
        return areSameVariable(3, mIncVar, mInIndex, mOutIndex);
    }

    int StrideSize()
    {
        if (mStride)
        {
            llvm::APSInt result;
            if (mStride->EvaluateAsInt(result, *mContext))
            {
                return result.getExtValue();
            }
            else 
            {
                return 0;    
            }
        }
        else
        {
            return 1;
        }
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
            // Check if we've had this map before
            // If we have then it has had 2 callbacks, making it almost certainly not a map
            if (mMapStatus.find(currentMap.mLoop) != mMapStatus.end())
            {
                removeViolation(MapLoop, this);
                return;
            }

            if (currentMap.IsMap())
            {
                PatternInfo p("Map", 0, currentMap.MapDump());
                auto stride = currentMap.StrideSize();
                if (stride == 0)
                    addViolation(MapLoop, this, p, "Strided Map of Unknown Stride Length");
                else if (stride == 1)
                    addViolation(MapLoop, this, p, "Non-Strided Map");
                else
                    addViolation(MapLoop, this, p, "Strided Map of Stride Length: " + std::to_string(stride));

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

        auto ForMatcher = ForLoop("Map", "", body); 

        auto WhileMatcher = WhileLoop("Map", "", body);

        
        addMatcher(ForMatcher);
        addMatcher(WhileMatcher);
    }

};

static RuleSet rules(new MapRule());
std::map<const Stmt*, MapInfo> MapRule::mMapStatus;
