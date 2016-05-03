#ifndef CAPA_RULESET_H
#define CAPA_RULESET_H

namespace CAPA
{

class RuleBase;

class RuleSet
{
public:
    explicit RuleSet(RuleBase* rule);
    static int numberOfRules();
    static RuleBase* getRuleAtIndex(int index);
};

} // end namespace CAPA

#endif
