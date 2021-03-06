#ifndef CAPA_ABSTRACTRESULTS_H
#define CAPA_ABSTRACTRESULTS_H

#include "CAPA/Results.h"

namespace CAPA
{

class ResultCollector;

class AbstractResults : public Results
{
protected:
    const ResultCollector& _resultCollector;

public:
    explicit AbstractResults(const ResultCollector& resultCollector);
    virtual ~AbstractResults() = default;

    virtual int numberOfViolations() const override;
    virtual int numberOfViolationsWithPriority(int priority) const override;

    virtual int numberOfFiles() const override;
    virtual int numberOfFilesWithViolations() const override;

    virtual int numberOfErrors() const override;
    virtual bool hasErrors() const override;

    virtual int numberOfWarnings() const override;
    virtual bool hasWarnings() const override;

    virtual int numberOfCheckerBugs() const override;
    virtual bool hasCheckerBugs() const override;
};

} // end namespace CAPA

#endif
