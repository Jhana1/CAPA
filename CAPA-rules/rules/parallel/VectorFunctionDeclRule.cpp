#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/MyASTUtil.h"
#include <iostream>
#include <sstream>
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
            if (markedIgnore(func, *SM))
                return;

            std::ostringstream source;
            source << func->getReturnType().getAsString() << " ";
            source << func->getNameInfo().getAsString() << "("; 
            for (int i = 0; i < func->getNumParams(); ++i) {
                source << func->parameters()[i]->getType().getAsString();
                source << " " << func->parameters()[i]->getQualifiedNameAsString();
                source << ", ";
            }
            source.seekp(-2, source.cur);
            source << ");";
            PatternInfo p("Vectorisable Function Declaration", 0, source.str());
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
