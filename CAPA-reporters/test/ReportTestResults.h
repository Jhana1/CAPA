#include "CAPA/ResultCollector.h"
#include "CAPA/AbstractResults.h"

namespace CAPA
{

class ReportTestResults : public AbstractResults
{
private:
    std::vector<Violation> _violations;

public:
    explicit ReportTestResults(const ResultCollector &resultCollector)
        : AbstractResults(resultCollector)
    {
    }

    std::vector<Violation> allViolations() const
    {
        return _violations;
    }

    const std::vector<Violation>& allErrors() const
    {
        return _violations;
    }

    const std::vector<Violation>& allWarnings() const
    {
        return _violations;
    }

    const std::vector<Violation>& allCheckerBugs() const
    {
        return _violations;
    }
};

}

CAPA::Results *getTestResults()
{
    CAPA::ResultCollector *resultCollector = CAPA::ResultCollector::getInstance();
    return new CAPA::ReportTestResults(*resultCollector);
}
