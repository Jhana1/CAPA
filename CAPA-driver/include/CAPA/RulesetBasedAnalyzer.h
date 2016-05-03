#ifndef CAPA_RULESETBASEDANALYZER_H
#define CAPA_RULESETBASEDANALYZER_H

#include "CAPA/Analyzer.h"

#include "CAPA/RuleBase.h"

namespace CAPA
{

class RulesetBasedAnalyzer : public Analyzer
{
private:
    std::vector<RuleBase *> _filteredRules;

public:
    explicit RulesetBasedAnalyzer(std::vector<RuleBase *> filteredRules);

    virtual void analyze(std::vector<clang::ASTContext*>& contexts) override;
};

} // end namespace CAPA

#endif
