#include <unistd.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <memory>

#include <clang/Tooling/CommonOptionsParser.h>

#include "CAPA/Analyzer.h"
#include "CAPA/CompilerInstance.h"
#include "CAPA/Driver.h"
#include "CAPA/GenericException.h"
#include "CAPA/Options.h"
#include "CAPA/RawResults.h"
#include "CAPA/Reporter.h"
#include "CAPA/ResultCollector.h"
#include "CAPA/RuleBase.h"
#include "CAPA/RuleSet.h"
#include "CAPA/RulesetFilter.h"
#include "CAPA/RulesetBasedAnalyzer.h"
#include "CAPA/UniqueResults.h"
#include "CAPA/Version.h"
#include "CAPA/ViolationSet.h"
#include "CAPA/Violation.h"

#include "reporters.h"
#include "rules.h"

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::tooling;

void consumeArgRulesPath()
{
    for (const auto& rulePath : CAPA::option::rulesPath())
    {
        dynamicLoadRules(rulePath);
    }
}

bool numberOfViolationsExceedThreshold(CAPA::Results *results)
{
    return results->numberOfViolationsWithPriority(1) > CAPA::option::maxP1() ||
        results->numberOfViolationsWithPriority(2) > CAPA::option::maxP2() ||
        results->numberOfViolationsWithPriority(3) > CAPA::option::maxP3();
}

ostream* outStream()
{
    if (!CAPA::option::hasOutputPath())
    {
        return &cout;
    }
    string output = CAPA::option::outputPath();
    auto out = new ofstream(output.c_str());
    if (!out->is_open())
    {
        throw CAPA::GenericException("cannot open report output file " + output);
    }
    return out;
}

void disposeOutStream(ostream* out)
{
    if (out && CAPA::option::hasOutputPath())
    {
        ofstream *fout = (ofstream *)out;
        fout->close();
    }
}

void listRules()
{
    cerr << "Enabled rules:\n";
    for (const std::string &ruleName : CAPA::option::rulesetFilter().filteredRuleNames())
    {
        cerr << "- " << ruleName << "\n";
    }
}

void printErrorLine(const char *errorMessage)
{
    cerr << endl << "CAPA: error: " << errorMessage << endl;
}

void printViolationsExceedThresholdError(const CAPA::Results *results)
{
    printErrorLine("violations exceed threshold");
    cerr << "P1=" << results->numberOfViolationsWithPriority(1)
        << "[" << CAPA::option::maxP1() << "] ";
    cerr << "P2=" << results->numberOfViolationsWithPriority(2)
        << "[" << CAPA::option::maxP2() << "] ";
    cerr << "P3=" << results->numberOfViolationsWithPriority(3)
        << "[" << CAPA::option::maxP3() << "] " <<endl;
}

std::unique_ptr<CAPA::Results> getResults()
{
    std::unique_ptr<CAPA::Results> results;
    if (CAPA::option::allowDuplicatedViolations())
    {
        results.reset(new CAPA::RawResults(*CAPA::ResultCollector::getInstance()));
    }
    else
    {
        results.reset(new CAPA::UniqueResults(*CAPA::ResultCollector::getInstance()));
    }
    return results;
}

enum ExitCode
{
    SUCCESS,
    RULE_NOT_FOUND,
    REPORTER_NOT_FOUND,
    ERROR_WHILE_PROCESSING,
    ERROR_WHILE_REPORTING,
    VIOLATIONS_EXCEED_THRESHOLD
};

int prepare()
{
    try
    {
        consumeArgRulesPath();
    }
    catch (const exception& e)
    {
        printErrorLine(e.what());
        return RULE_NOT_FOUND;
    }
    if (CAPA::RuleSet::numberOfRules() <= 0)
    {
        printErrorLine("no rule loaded");
        return RULE_NOT_FOUND;
    }
    try
    {
        loadReporter();
    }
    catch (const exception& e)
    {
        printErrorLine(e.what());
        return REPORTER_NOT_FOUND;
    }

    return SUCCESS;
}

static void CAPAVersionPrinter()
{
    cout << "OCLint (http://CAPA.org/):\n";
    cout << "  OCLint version " << CAPA::Version::identifier() << ".\n";
    cout << "  Built " << __DATE__ << " (" << __TIME__ << ").\n";
}

extern llvm::cl::OptionCategory OCLintOptionCategory;

int main(int argc, const char **argv)
{
    llvm::cl::AddExtraVersionPrinter(&CAPAVersionPrinter);
    CommonOptionsParser optionsParser(argc, argv, OCLintOptionCategory);
    CAPA::option::process(argv[0]);

    int prepareStatus = prepare();
    if (prepareStatus)
    {
        return prepareStatus;
    }

    if (CAPA::option::showEnabledRules())
    {
        listRules();
    }

    CAPA::RulesetBasedAnalyzer analyzer(CAPA::option::rulesetFilter().filteredRules());
    CAPA::Driver driver;
    try
    {
        driver.run(optionsParser.getCompilations(), optionsParser.getSourcePathList(), analyzer);
    }
    catch (const exception& e)
    {
        printErrorLine(e.what());
        return ERROR_WHILE_PROCESSING;
    }

    std::unique_ptr<CAPA::Results> results(std::move(getResults()));

    try
    {
        ostream *out = outStream();
        reporter()->report(results.get(), *out);
        disposeOutStream(out);
    }
    catch (const exception& e)
    {
        printErrorLine(e.what());
        return ERROR_WHILE_REPORTING;
    }

    return SUCCESS;
}
