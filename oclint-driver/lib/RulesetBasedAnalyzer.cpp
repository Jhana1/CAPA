#include "CAPA/RulesetBasedAnalyzer.h"

#include <utility>

#include "CAPA/Logger.h"
#include "CAPA/ResultCollector.h"
#include "CAPA/RuleBase.h"
#include "CAPA/RuleCarrier.h"
#include "CAPA/RuleSet.h"
#include "CAPA/ViolationSet.h"

using namespace CAPA;

RulesetBasedAnalyzer::RulesetBasedAnalyzer(std::vector<RuleBase*> filteredRules)
    : _filteredRules(std::move(filteredRules))
{
}

void RulesetBasedAnalyzer::analyze(std::vector<clang::ASTContext *> &contexts)
{
    for (const auto& context : contexts)
    {
        LOG_VERBOSE("Analyzing ");
        auto violationSet = new ViolationSet();
        auto carrier = new RuleCarrier(context, violationSet);
        LOG_VERBOSE(carrier->getMainFilePath().c_str());
        for (RuleBase *rule : _filteredRules)
        {
            rule->takeoff(carrier);
        }
        ResultCollector *results = ResultCollector::getInstance();
        results->add(violationSet);
        LOG_VERBOSE_LINE(" - Done");
    }
}
