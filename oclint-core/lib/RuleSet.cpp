#include "CAPA/RuleSet.h"

#include <vector>

#include "CAPA/RuleBase.h"

using namespace CAPA;

static std::vector<RuleBase*>* _rules = nullptr;

RuleSet::RuleSet(RuleBase* rule)
{
    if (_rules == nullptr)
    {
        _rules = new std::vector<RuleBase*>();
    }
    _rules->push_back(rule);
}

int RuleSet::numberOfRules()
{
    return _rules == nullptr ? 0 : _rules->size();
}

RuleBase* RuleSet::getRuleAtIndex(int index)
{
    if (index >= numberOfRules())
    {
        return nullptr; // Better throwing an exception
    }
    return _rules->at(index);
}
