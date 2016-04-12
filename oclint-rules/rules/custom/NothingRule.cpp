#include "oclint/AbstractASTMatcherRule.h"
#include "oclint/RuleSet.h"
#include <iostream>
#include <cstdarg>

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace oclint;

class NothingRule : public AbstractASTMatcherRule
{
protected:
    bool areSameVariable(int n, ...)
    {
        va_list args;
        va_start(args,n);
        VarDecl *tmp = va_arg(args, VarDecl*)->getCanonicalDecl();
        for (int i = 1; i < n; ++i)
        {
            VarDecl* node = va_arg(args, VarDecl*);
            if (!node || node->getCanonicalDecl() != tmp)
                return false;
        }
        return true; 
    }

    template <typename T>
    std::string node2str(const T *node, SourceManager &sm) 
    {
        string text = Lexer::getSourceText(CharSourceRange::getTokenRange(node->getSourceRange()), 
                                           sm, LangOptions(), 0);
        if (text.at(text.size()-1) == ',')
            return Lexer::getSourceText(CharSourceRange::getCharRange(node->getSourceRange()), 
                                        sm, LangOptions(), 0);
        else
            return text;
    }
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
        
        if (MapLoop)
        {
            MapLoop->dumpColor();
            auto MapInitVar  = Result.Nodes.getNodeAs<VarDecl>("MapInitVar");
            auto MapIncVar   = Result.Nodes.getNodeAs<VarDecl>("MapIncVar");
            auto MapVecBase  = Result.Nodes.getNodeAs<VarDecl>("MapVecBase");
            auto MapVecIndex = Result.Nodes.getNodeAs<VarDecl>("MapVecIndex");
            auto MapAssign   = Result.Nodes.getNodeAs<BinaryOperator>("MapVecAssign");

            std::cout << "MapInitVar: " << MapInitVar->getCanonicalDecl() << std::endl;
            std::cout << "MapIncVar: " << MapIncVar->getCanonicalDecl() << std::endl;
            std::cout << "MapVecBase: " << MapVecBase->getCanonicalDecl() << std::endl;
            std::cout << "MapVecIndex: " << MapVecIndex->getCanonicalDecl() << std::endl;

            if (areSameVariable(3, MapInitVar, MapIncVar, MapVecIndex))
            {
                std::cout << "Map Detected" << std::endl;
                std::string out = node2str(MapLoop, Result.Context->getSourceManager());
                std::cout << out << std::endl;
            }
        }
       
    }

    virtual void setUpMatcher() override
    {
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
                        varDecl().bind("MapVecBase")))))),
                    hasIndex(hasDescendant(declRefExpr(to(varDecl(hasType(
                        isInteger())).bind("MapVecIndex")))))))).bind("MapVecAssign"))))
                        .bind("MapLoop");
        
        addMatcher(MapMatcher);
        
        // Matches on For Loops with counter initialised in the init, with an array element
        // assignment within the body of the loop
        auto map1 =            
        forStmt(
            hasLoopInit(declStmt(hasSingleDecl(varDecl(
                hasInitializer(integerLiteral(equals(0)))).bind("initVarName")))),
            hasIncrement(unaryOperator(
                hasOperatorName("++"),
                hasUnaryOperand(declRefExpr(to(varDecl(hasType(isInteger())).bind("incVarName")))))),
            hasCondition(binaryOperator(
                hasOperatorName("<"),
                hasLHS(ignoringParenImpCasts(declRefExpr(
                    to(varDecl(hasType(isInteger())).bind("condVarName"))))),
                hasRHS(expr(hasType(isInteger()))))),
            hasBody(has(binaryOperator(
                hasLHS(arraySubscriptExpr(
                    hasBase(implicitCastExpr(hasSourceExpression(
                        declRefExpr(to(varDecl().bind("arrBase")))))),
                    hasIndex(hasDescendant(declRefExpr(to(varDecl(hasType(
                        isInteger())).bind("arrIndex"))))))),
                    hasOperatorName("=")).bind("Arr")))
        ).bind("forLoop");

        // Matches on For Loops with counter not initialised in the init with an array element
        // assignment within the body of the loop
        /*
        forStmt(
            hasLoopInit(binaryOerator(
                hasLHS(ignoringParenImpCasts(declRefExpr(
                    to(varDecl(hasType(isInteger())).bind("initVarName1"))))))),
            hasIncrement(unaryOperator(
                hasOperatorName("++"),
                hasUnaryOperand(declRefExpr(
                    to(varDecl(hasType(isInteger())).bind("incVarName1")))))),
            hasCondition(binaryOperator(
                hasOperatorName("<"),
                        hasLHS(ignoringParenImpCasts(declRefExpr(
        */                    
        // General Map Matcher
        

    }

};

static RuleSet rules(new NothingRule());
