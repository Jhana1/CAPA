#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
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
        return "vector function decl";
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
        std::cout << "CAUGHT A FUNCTION" << std::endl;
    }

    virtual void setUpMatcher() override
    {
        auto Matcher = functionDecl(allOf(
                           anyOf(hasAnyParameter(hasType(arrayType())),
                                 hasAnyParameter(hasType(pointerType()))),
                           hasAnyParameter(hasType(isInteger()))));
        addMatcher(Matcher);
    }

};

static RuleSet rules(new VectorFunctionDeclRule());
