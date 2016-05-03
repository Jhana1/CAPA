#include "CAPA/Results.h"
#include "CAPA/Reporter.h"
#include "CAPA/RuleBase.h"
#include "CAPA/ViolationSet.h"

using namespace CAPA;

class XcodeReporter : public Reporter
{
public:
    virtual const std::string name() const override
    {
        return "xcode";
    }

    virtual void report(Results* results, std::ostream& out) override
    {
        // Compiler warnings, errors and clang static analyzer results
        // can be retrieved from Xcode directly, so we only need to
        // output violations that is emitted by CAPA.

        for (const auto& violation : results->allViolations())
        {
            writeViolation(out, violation);
            out << std::endl;
        }
    }

    void writeViolation(std::ostream &out, const Violation &violation)
    {
        out << violation.path << ":" << violation.startLine << ":" << violation.startColumn;
        const RuleBase *rule = violation.rule;
        out << ": warning: " << rule->name();
        out << " [" << rule->category() << "|P" << rule->priority() << "]";
        out << " " << violation.message;
    }
};

extern "C" Reporter* create()
{
  return new XcodeReporter();
}
