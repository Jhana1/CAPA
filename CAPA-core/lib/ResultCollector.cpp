#include "CAPA/ResultCollector.h"
#include "CAPA/RuleBase.h"
#include "CAPA/Violation.h"
#include "CAPA/ViolationSet.h"

static CAPA::ResultCollector *_singleton = nullptr;

namespace CAPA {

ResultCollector* ResultCollector::getInstance()
{
    if (_singleton == nullptr)
    {
        _singleton = new ResultCollector();
    }
    return _singleton;
}

ResultCollector::ResultCollector()
    : _compilerErrorSet(new ViolationSet)
    , _compilerWarningSet(new ViolationSet)
    , _clangStaticCheckerBugSet(new ViolationSet)
{
}

ResultCollector::~ResultCollector()
{
}

void ResultCollector::add(ViolationSet *violationSet)
{
    _collection.push_back(violationSet);
}

const std::vector<ViolationSet*>& ResultCollector::getCollection() const
{
    return _collection;
}

void ResultCollector::addError(const Violation& violation)
{
    _compilerErrorSet->addViolation(violation);
}

ViolationSet* ResultCollector::getCompilerErrorSet() const
{
    return _compilerErrorSet.get();
}

void ResultCollector::addWarning(const Violation& violation)
{
    _compilerWarningSet->addViolation(violation);
}

ViolationSet* ResultCollector::getCompilerWarningSet() const
{
    return _compilerWarningSet.get();
}

void ResultCollector::addCheckerBug(const Violation& violation)
{
    _clangStaticCheckerBugSet->addViolation(violation);
}

ViolationSet* ResultCollector::getClangStaticCheckerBugSet() const
{
    return _clangStaticCheckerBugSet.get();
}

} // end namespace CAPA
