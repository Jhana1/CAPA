#include "CAPA/AbstractSourceCodeReaderRule.h"
#include "CAPA/RuleConfiguration.h"
#include "CAPA/RuleSet.h"
#include "CAPA/util/StdUtil.h"

using namespace std;
using namespace CAPA;

class LongLineRule : public AbstractSourceCodeReaderRule
{
public:
    virtual const string name() const override
    {
        return "long line";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "size";
    }

    virtual void eachLine(int lineNumber, string line) override
    {
        int threshold = RuleConfiguration::intForKey("LONG_LINE", 100);
        int currentLineSize = line.size();
        if (currentLineSize > threshold)
        {
            string description = "Line with " + toString<int>(currentLineSize) +
                " characters exceeds limit of " + toString<int>(threshold);
            addViolation(lineNumber, 1, lineNumber, currentLineSize, this, description);
        }
    }
};

static RuleSet rules(new LongLineRule());
