#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/MyASTUtil.h"
#include <iostream>
using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace CAPA;

class VectorFunctionDeclRule : public AbstractASTMatcherRule
{
public:
    virtual const string name() const override
    {
        return "Vectorisable Function Declaration";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "parallel";
    }

    virtual void callback(const MatchFinder::MatchResult &result) override
    {
        auto func = result.Nodes.getNodeAs<FunctionDecl>("Function");
        SourceManager *SM = &result.Context->getSourceManager();
        if (func)
        {
            PatternInfo p("Vectorisable Function Declaration", node2str(func, *SM));
            addViolation(func, this, p, "Function Declaration");
        }

    }

    virtual void setUpMatcher() override
    {
        auto Matcher = functionDecl(allOf(
                           anyOf(hasAnyParameter(hasType(arrayType())),
                                 hasAnyParameter(hasType(pointerType()))),
                           hasAnyParameter(hasType(asString("size_t"))))).bind("Function");
        addMatcher(Matcher);
    }

};

static RuleSet rules(new VectorFunctionDeclRule());
