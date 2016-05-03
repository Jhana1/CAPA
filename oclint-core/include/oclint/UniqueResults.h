#ifndef CAPA_UNIQUERESULTS_H
#define CAPA_UNIQUERESULTS_H

#include "CAPA/AbstractResults.h"

namespace CAPA
{

class ResultCollector;

class UniqueResults : public AbstractResults
{
private:
    mutable std::vector<Violation> _violations;
    mutable std::vector<Violation> _errors;
    mutable std::vector<Violation> _warnings;
    mutable std::vector<Violation> _checkerBugs;

public:
    explicit UniqueResults(const ResultCollector& resultCollector);

    std::vector<Violation> allViolations() const override;
    const std::vector<Violation>& allErrors() const override;
    const std::vector<Violation>& allWarnings() const override;
    const std::vector<Violation>& allCheckerBugs() const override;
};

} // end namespace CAPA

#endif
