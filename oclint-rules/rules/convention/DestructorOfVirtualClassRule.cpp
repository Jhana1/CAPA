#include "CAPA/AbstractASTVisitorRule.h"
#include "CAPA/RuleSet.h"

using namespace clang;

static bool hasVirtualDestructor(const CXXRecordDecl& cxxRecordDecl)
{
    const CXXDestructorDecl* cxxDestructorDecl = cxxRecordDecl.getDestructor();

    return cxxDestructorDecl != nullptr && cxxDestructorDecl->isVirtual();
}

static std::string getMessageViolation(const CXXRecordDecl& cxxRecordDecl)
{
    const std::string& className = cxxRecordDecl.getNameAsString();
    return "class " + className + " should have a virtual destructor ~" + className + "()";
}

class DestructorOfVirtualClassRule :
    public CAPA::AbstractASTVisitorRule<DestructorOfVirtualClassRule>
{
public:
    virtual const std::string name() const override
    {
        return "destructor of virtual class";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const std::string category() const override
    {
        return "convention";
    }

    unsigned int supportedLanguages() const override
    {
        return CAPA::LANG_CXX;
    }

    bool VisitCXXRecordDecl(const CXXRecordDecl* cxxRecordDecl)
    {
        if (cxxRecordDecl != nullptr
            && cxxRecordDecl->hasDefinition()
            && cxxRecordDecl->isPolymorphic()
            && !hasVirtualDestructor(*cxxRecordDecl))
        {
            addViolation(cxxRecordDecl, this, getMessageViolation(*cxxRecordDecl));
        }
        return true;
    }
};

static CAPA::RuleSet rules(new DestructorOfVirtualClassRule());
