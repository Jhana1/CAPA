#include "CAPA/UniqueResults.h"

#include <unordered_set>

#include "CAPA/ResultCollector.h"
#include "CAPA/RuleBase.h"
#include "CAPA/Violation.h"

namespace
{

class ViolationHash
{
public:
    std::size_t operator()(const CAPA::Violation& violation) const
    {
        std::size_t hash1 = std::hash<const CAPA::RuleBase*>()(violation.rule);
        std::size_t hash2 = std::hash<std::string>()(violation.path);
        std::size_t hash3 = std::hash<std::string>()(violation.message);
        std::size_t hash4 = violation.startLine
            ^ (violation.startColumn << 2)
            ^ (violation.endLine << 4)
            ^ (violation.endColumn << 8);
        return hash1 ^ (hash2 << 1) ^ (hash2 << 2) ^ (hash3 << 3) ^ (hash4 << 4);
    }
};

std::vector<CAPA::Violation> removeViolationDuplications(
  std::vector<CAPA::Violation> originalViolations)
{
    std::vector<CAPA::Violation> violations;
    std::unordered_set<CAPA::Violation, ViolationHash> set;

    for (const auto& violation : originalViolations)
    {
        if (set.insert(violation).second)
        {
            violations.push_back(violation);
        }
    }

    return violations;
}

} // end namespace


namespace CAPA
{

UniqueResults::UniqueResults(const ResultCollector &resultCollector)
    : AbstractResults(resultCollector)
{
}

std::vector<CAPA::Violation> UniqueResults::allViolations() const
{
    if (!_violations.empty())
    {
        return _violations;
    }

    std::vector<Violation> violations;
    for (const auto& violationSet : _resultCollector.getCollection())
    {
        for (const auto& violation : violationSet->getViolations())
        {
            violations.push_back(violation);
        }
    }
    _violations = removeViolationDuplications(violations);
    return _violations;
}

const std::vector<Violation>& UniqueResults::allErrors() const
{
    if (!_errors.empty())
    {
        return _errors;
    }

    _errors = removeViolationDuplications(
        _resultCollector.getCompilerErrorSet()->getViolations());
    return _errors;
}

const std::vector<Violation>& UniqueResults::allWarnings() const
{
    if (!_warnings.empty())
    {
        return _warnings;
    }

    _warnings = removeViolationDuplications(
        _resultCollector.getCompilerWarningSet()->getViolations());
    return _warnings;
}

const std::vector<Violation>& UniqueResults::allCheckerBugs() const
{
    if (!_checkerBugs.empty())
    {
        return _checkerBugs;
    }

    _checkerBugs = removeViolationDuplications(
        _resultCollector.getClangStaticCheckerBugSet()->getViolations());
    return _checkerBugs;
}

} // end namespace CAPA
