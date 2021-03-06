#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/MyASTUtil.h"
#include "CAPA/helper/MatcherHelper.h"
#include <iostream>

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace CAPA;

class VectorisableRule : public AbstractASTMatcherRule
{
public:
    virtual const string name() const override
    {
        return "Vectorisable region";
    }

    virtual int priority() const override
    {
        return 5;
    }

    virtual const string category() const override
    {
        return "parallel";
    }

    virtual void callback(const MatchFinder::MatchResult &Result) override
    {
        auto Context = Result.Context;
        auto Loop = Result.Nodes.getStmtAs<Stmt>("Loop");
        auto Function = Result.Nodes.getNodeAs<FunctionDecl>("Function");
        if (Loop)
        {
            if (markedIgnore(Function, Context->getSourceManager()))
                return;
            PatternInfo p("Vectorisable", 0,  node2str(Loop, Result.Context->getSourceManager()), priority());
            if (p.dumpSource() == "") { return; } // No Valid Source
            addViolation(Loop, this, p, "Generally vectorisable region of code");
        }
    }

    virtual void setUpMatcher() override
    {
        auto unless = anyOf(
                        HasDescendant(ifStmt(anything())),
                        HasDescendant(switchStmt(anything())),
                        HasDescendant(gotoStmt(anything())),
                        hasAncestor(forStmt(anyOf(
                            hasAncestor(forStmt(anything())),
                            hasAncestor(whileStmt()),
                            hasAncestor(doStmt())))),
                        hasAncestor(whileStmt(anyOf(
                            hasAncestor(forStmt(anything())),
                            hasAncestor(whileStmt(anything())),
                            hasAncestor(doStmt(anything()))))),
                        hasAncestor(doStmt(anyOf(
                            hasAncestor(forStmt(anything())),
                            hasAncestor(whileStmt()),
                            hasAncestor(doStmt()))))
                        );

        auto body = anything();

        auto ForMatcher = FunctionWrap(ForLoopUnless("Loop", body, unless));
        auto WhileMatcher = FunctionWrap(WhileLoopUnless("Loop", body, unless));

        addMatcher(ForMatcher);
        addMatcher(WhileMatcher);
    }

};

static RuleSet rules(new VectorisableRule());
