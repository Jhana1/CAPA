#ifndef CAPA_RESULTCOLLECTOR_H
#define CAPA_RESULTCOLLECTOR_H

#include "CAPA/Violation.h"

#include <vector>
#include <memory>

namespace CAPA
{

class ViolationSet;

class ResultCollector
{
protected:
    ResultCollector();
    ~ResultCollector();

public:
    static ResultCollector* getInstance();

private:
    std::vector<ViolationSet*> _collection;
    std::unique_ptr<ViolationSet> _compilerErrorSet;
    std::unique_ptr<ViolationSet> _compilerWarningSet;
    std::unique_ptr<ViolationSet> _clangStaticCheckerBugSet;

public:
    void add(ViolationSet *violationSet);

    const std::vector<ViolationSet*>& getCollection() const;

    void addError(const Violation& violation);
    ViolationSet* getCompilerErrorSet() const;

    void addWarning(const Violation& violation);
    ViolationSet* getCompilerWarningSet() const;

    void addCheckerBug(const Violation& violation);
    ViolationSet* getClangStaticCheckerBugSet() const;
};

} // end namespace CAPA

#endif
