#ifndef CAPA_ABSTRACTASTRULEBASE_H
#define CAPA_ABSTRACTASTRULEBASE_H

#include <clang/AST/AST.h>
#include "CAPA/PatternInfo.h"
#include "CAPA/RuleBase.h"

namespace CAPA
{

enum LanguageSupportFlags
{
    LANG_C    = 1 << 0,
    LANG_CXX  = 1 << 1,
    LANG_OBJC = 1 << 2
};

class AbstractASTRuleBase : public RuleBase
{
protected:
    void addViolation(clang::SourceLocation startLocation,
        clang::SourceLocation endLocation, RuleBase *rule, const PatternInfo &patternInfo,
        const std::string& message = "");

    void addViolation(const clang::Decl *decl, RuleBase *rule, const PatternInfo &patternInfo,
            const std::string& message = "");
    void addViolation(const clang::Stmt *stmt, RuleBase *rule, const PatternInfo &patternInfo,
            const std::string& message = "");

private:
    bool supportsC() const;
    bool supportsCXX() const;
    bool supportsObjC() const;

protected:
    virtual unsigned int supportedLanguages() const;
    bool isLanguageSupported() const;

public:
    virtual ~AbstractASTRuleBase();
};

} // end namespace CAPA

#endif
