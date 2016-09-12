#include "CAPA/BenchmarkSet.h"
#include "CAPA/PatternInfo.h"
#include "CAPA/Results.h"
#include "CAPA/Reporter.h"
#include "CAPA/RuleBase.h"
#include "CAPA/Version.h"
#include "CAPA/ViolationSet.h"
#include "nlohmann/json.hpp"
#include "CAPA/util/Colors.h"
using namespace CAPA;

class TextReporter : public Reporter
{
public:
    virtual const std::string name() const override
    {
        return "text";
    }

    virtual void report(Results* results, BenchmarkSet &benchmarks,  std::ostream& out) override
    {
        if (results->hasErrors())
        {
            writeCompilerDiagnostics(out, results->allErrors(),
                "Compiler Errors:\n(please be aware that these errors "
                "will prevent CAPA from analyzing this source code)");
        }
        if (results->hasWarnings())
        {
            writeCompilerDiagnostics(out, results->allWarnings(), "Compiler Warnings:");
        }
        out << std::endl << std::endl;
        writeHeader(out);
        out << std::endl << std::endl;
        writeSummary(out, *results);
        out << std::endl << std::endl;
        writeViolations(out, benchmarks, results->allViolations());
        out << std::endl;
        writeFooter(out, Version::identifier());
        out << std::endl;
    }

    void writeHeader(std::ostream &out)
    {
        out << Color::FG_WHITE << "CAPA Report";
    }

    void writeFooter(std::ostream &out, std::string version)
    {
        out << Color::FG_WHITE << "[CAPA v" << version << "]";
    }

    void writeSummary(std::ostream &out, Results &results)
    {
        out << Color::FG_WHITE << "Summary: TotalFiles = " << results.numberOfFiles() << " ";
        out << "Files With Improvements = " << results.numberOfFilesWithViolations() << " ";
    }

    void writeViolation(std::ostream &out, BenchmarkSet &benchmarks, const Violation &violation)
    {
        out.precision(2);
        out << Color::FG_MAGENTA << violation.path      << Color::FG_DEFAULT << ":";
        out << Color::FG_CYAN    << violation.startLine << Color::FG_DEFAULT << ":";
        out << Color::FG_CYAN    << violation.startColumn << std::endl;
        const RuleBase *rule = violation.rule;
        out << Color::FG_YELLOW << "Pattern: "    << Color::FG_WHITE    << rule->name();
        out << Color::FG_YELLOW << " Priority: "  << Color::FG_WHITE    << rule->priority();
        out << Color::FG_YELLOW << " Info: "      << Color::FG_CYAN     << violation.message;

        if (benchmarks.Exists(violation.patternInfo.pattern))
        {
            double speedup = benchmarks.Speedup(violation.patternInfo.pattern, 
                                                violation.patternInfo.dimension);
            out << std::endl << Color::FG_YELLOW << "Potential Speedup: ";
            if (speedup < 1)
            {
                out << Color::FG_RED << std::fixed << speedup;
            }
            else
            {
                out << Color::FG_GREEN << std::fixed << speedup;
            }
            out << "X";
        }
        out << Color::FG_DEFAULT << Color::BG_DEFAULT << std::endl;
        out << violation.patternInfo.dumpSource() << std::endl;
    }

    void writeViolations(std::ostream &out, BenchmarkSet &benchmarks, std::vector<Violation> violations)
    {
        for (const auto& violation : violations)
        {
            writeViolation(out, benchmarks, violation);
            out << std::endl;
        }
    }

    void writeCompilerErrorOrWarning(std::ostream &out, const Violation &violation)
    {
        out << violation.path << ":" << violation.startLine << ":" << violation.startColumn;
        out << ": " << violation.message;
    }

    void writeCompilerDiagnostics(std::ostream &out, std::vector<Violation> violations,
        std::string headerText)
    {
        out << std::endl << headerText << std::endl << std::endl;
        for (const auto& violation : violations)
        {
            writeCompilerErrorOrWarning(out, violation);
            out << std::endl;
        }
    }
};

extern "C" Reporter* create()
{
  return new TextReporter();
}
