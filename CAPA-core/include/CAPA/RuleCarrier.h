#ifndef CAPA_RULECARRIER_H
#define CAPA_RULECARRIER_H

#include <string>

namespace clang
{
    class ASTContext;
    class SourceManager;
    class TranslationUnitDecl;
}

#include "CAPA/ViolationSet.h"
#include "CAPA/PatternInfo.h"

namespace CAPA
{

class RuleCarrier
{
private:
    ViolationSet *_violationSet;
    clang::ASTContext *_astContext;

public:
    RuleCarrier(clang::ASTContext *astContext, ViolationSet *violationSet);
    clang::ASTContext* getASTContext();
    clang::SourceManager& getSourceManager();
    std::string getMainFilePath();
    clang::TranslationUnitDecl* getTranslationUnitDecl();

    void addViolation(std::string filePath, int startLine, int startColumn,
        int endLine, int endColumn, RuleBase *rule, const PatternInfo &patternInfo,
        const std::string& message = "");
};

} // end namespace CAPA

#endif
