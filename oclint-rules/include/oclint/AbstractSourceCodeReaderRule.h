#ifndef CAPA_ABSTRACTSOURCECODEREADERRULE_H
#define CAPA_ABSTRACTSOURCECODEREADERRULE_H

#include "CAPA/RuleBase.h"

namespace CAPA
{

class AbstractSourceCodeReaderRule : public RuleBase
{
protected:
    virtual void apply() override;

    void addViolation(int startLine, int startColumn,
        int endLine, int endColumn, RuleBase *rule, const std::string& message = "");

public:
    virtual ~AbstractSourceCodeReaderRule();

    virtual void eachLine(int lineNumber, std::string line) = 0;
};

} // end namespace CAPA

#endif
