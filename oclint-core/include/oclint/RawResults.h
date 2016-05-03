#ifndef CAPA_RAWRESULTS_H
#define CAPA_RAWRESULTS_H

#include "CAPA/AbstractResults.h"

namespace CAPA
{

class ResultCollector;

class RawResults : public AbstractResults
{
public:
    explicit RawResults(const ResultCollector& resultCollector);

    std::vector<Violation> allViolations() const override;
    const std::vector<Violation>& allErrors() const override;
    const std::vector<Violation>& allWarnings() const override;
    const std::vector<Violation>& allCheckerBugs() const override;
};

} // end namespace CAPA

#endif
