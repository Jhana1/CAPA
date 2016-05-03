#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace CAPA;

namespace clang {
namespace ast_matchers {
AST_MATCHER(VarDecl, isStaticDataMember)
{
    return Node.isStaticDataMember();
}

AST_MATCHER(CXXMethodDecl, isStatic)
{
    return Node.isStatic();
}
} // namespace ast_matchers
} // namespace clang

class AvoidPrivateStaticMembersRule : public AbstractASTMatcherRule
{
public:
    virtual const string name() const override
    {
        return "avoid private static members";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "design";
    }

    virtual void callback(const MatchFinder::MatchResult &result) override
    {
        if (auto field = result.Nodes.getNodeAs<VarDecl>("field"))
        {
            addViolation(field, this);
        }
        if (auto method = result.Nodes.getNodeAs<CXXMethodDecl>("cxxMethod"))
        {
            addViolation(method, this);
        }
    }

    virtual void setUpMatcher() override
    {
        addMatcher(varDecl(isPrivate(), isStaticDataMember()).bind("field"));
        addMatcher(methodDecl(isPrivate(), isStatic()).bind("cxxMethod"));
    }
};

static RuleSet rules(new AvoidPrivateStaticMembersRule());
