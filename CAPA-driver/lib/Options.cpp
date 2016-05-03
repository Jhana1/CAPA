#include "CAPA/Options.h"

#include <unistd.h>

#include <llvm/Option/OptTable.h>
#include <llvm/Option/Option.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Program.h>
#include <clang/Driver/Options.h>
#include <clang/Tooling/CommonOptionsParser.h>

#include "CAPA/ConfigFile.h"
#include "CAPA/RuleConfiguration.h"

using namespace CAPA;

llvm::cl::OptionCategory CAPAOptionCategory("CAPA options");

/* ----------------
   input and output
   ---------------- */

static llvm::cl::opt<std::string> argOutput("o",
    llvm::cl::desc("Write output to <path>"),
    llvm::cl::value_desc("path"),
    llvm::cl::init("-"),
    llvm::cl::cat(CAPAOptionCategory));

/* --------------------
   CAPA configuration
   -------------------- */

static llvm::cl::opt<std::string> argReportType("report-type",
    llvm::cl::desc("Change output report type"),
    llvm::cl::value_desc("name"),
    llvm::cl::init("text"),
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::list<std::string> argRulesPath("R",
    llvm::cl::Prefix,
    llvm::cl::desc("Add directory to rule loading path"),
    llvm::cl::value_desc("directory"),
    llvm::cl::ZeroOrMore,
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::list<std::string> argRuleConfiguration("rc",
    llvm::cl::desc("Override the default behavior of rules"),
    llvm::cl::value_desc("parameter>=<value"),
    llvm::cl::ZeroOrMore,
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::list<std::string> argEnabledRules("rule",
    llvm::cl::desc("Explicitly pick rules"),
    llvm::cl::value_desc("rule name"),
    llvm::cl::ZeroOrMore,
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::list<std::string> argDisabledRules("disable-rule",
    llvm::cl::desc("Disable rules"),
    llvm::cl::value_desc("rule name"),
    llvm::cl::ZeroOrMore,
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::opt<bool> argListEnabledRules("list-enabled-rules",
    llvm::cl::desc("List enabled rules"),
    llvm::cl::init(false),
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::opt<int> argMaxP1("max-priority-1",
    llvm::cl::desc("The max allowed number of priority 1 violations"),
    llvm::cl::value_desc("threshold"),
    llvm::cl::init(0),
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::opt<int> argMaxP2("max-priority-2",
    llvm::cl::desc("The max allowed number of priority 2 violations"),
    llvm::cl::value_desc("threshold"),
    llvm::cl::init(10),
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::opt<int> argMaxP3("max-priority-3",
    llvm::cl::desc("The max allowed number of priority 3 violations"),
    llvm::cl::value_desc("threshold"),
    llvm::cl::init(20),
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::opt<bool> argGlobalAnalysis("enable-global-analysis",
    llvm::cl::desc("Compile every source, and analyze across global contexts "
        "(depends on number of source files, could results in high memory load)"),
    llvm::cl::init(false),
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::opt<bool> argClangChecker("enable-clang-static-analyzer",
    llvm::cl::desc("Enable Clang Static Analyzer, and integrate results into CAPA report"),
    llvm::cl::init(false),
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::opt<bool> argDuplications("allow-duplicated-violations",
    llvm::cl::desc("Allow duplicated violations in the CAPA report"),
    llvm::cl::init(false),
    llvm::cl::cat(CAPAOptionCategory));
static llvm::cl::opt<bool> argEnableVerbose("verbose",
    llvm::cl::desc("Enable verbose output"),
    llvm::cl::init(false),
    llvm::cl::Hidden,
    llvm::cl::cat(CAPAOptionCategory));

/* -------------
   libTooling cl
   ------------- */

static llvm::cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp MoreHelp(
    "For more information, please visit http://CAPA.org\n"
);
static std::unique_ptr<llvm::opt::OptTable> Options(clang::driver::createDriverOptTable());

/* -------
   options
   ------- */

static CAPA::RulesetFilter filter;
static std::string absoluteWorkingPath("");
static std::string executablePath("");

template <typename T>
void updateArgIfSet(llvm::cl::opt<T> &argValue, const llvm::Optional<T> &configValue)
{
    if (configValue.hasValue() && argValue.getNumOccurrences() == 0)
    {
        argValue.setValue(configValue.getValue());
    }
}

static std::vector<std::string> configFilePaths()
{
    std::vector<std::string> paths;
    paths.push_back(CAPA::option::etcPath() + "/CAPA");
    if (CAPA::option::homePath().length() > 0) {
        paths.push_back(CAPA::option::homePath() + "/.CAPA");
    }
    paths.push_back(CAPA::option::workingPath() + "/.CAPA");
    return paths;
}

static void processConfigFile(const std::string &path)
{
    CAPA::option::ConfigFile config(path);
    for (const CAPA::option::RuleConfigurationPair &ruleConfig : config.ruleConfigurations())
    {
        CAPA::RuleConfiguration::addConfiguration(ruleConfig.key(), ruleConfig.value());
    }
    for (const llvm::StringRef &rulePath : config.rulePaths())
    {
        argRulesPath.push_back(rulePath.str());
    }
    std::vector<std::string> enableRules;
    for (auto rule : config.rules())
    {
        enableRules.push_back(rule.str());
    }
    filter.enableRules(enableRules.begin(), enableRules.end());
    std::vector<std::string> disableRules;
    for (auto rule : config.disableRules())
    {
        disableRules.push_back(rule.str());
    }
    filter.disableRules(disableRules.begin(), disableRules.end());

    updateArgIfSet(argOutput, config.output());
    updateArgIfSet(argReportType, config.reportType());
    updateArgIfSet(argMaxP1, config.maxP1());
    updateArgIfSet(argMaxP2, config.maxP2());
    updateArgIfSet(argMaxP3, config.maxP3());
    updateArgIfSet(argClangChecker, config.clangChecker());
    updateArgIfSet(argDuplications, config.allowDuplicatedViolations());
}

static void processConfigFiles()
{
    std::vector<std::string> paths = configFilePaths();
    for_each(paths.begin(), paths.end(), processConfigFile);
}

static void preserveWorkingPath()
{
    char path[300];
    if (getcwd(path, 300))
    {
        absoluteWorkingPath = std::string(path);
    }
}

static void preserveExecutablePath(const char *argv)
{
    llvm::SmallString<128> installedPath(argv);
    if (llvm::sys::path::filename(installedPath) == installedPath)
    {
        auto intermediatePath =
            llvm::sys::findProgramByName(llvm::sys::path::filename(installedPath.str()));
        if (intermediatePath)
        {
            installedPath = *intermediatePath;
        }
    }
    llvm::sys::fs::make_absolute(installedPath);
    installedPath = llvm::sys::path::parent_path(installedPath);
    executablePath = std::string(installedPath.c_str());
}

void CAPA::option::process(const char *argv)
{
    preserveWorkingPath();
    preserveExecutablePath(argv);

    processConfigFiles();
    for (unsigned i = 0; i < argRuleConfiguration.size(); ++i)
    {
        std::string configuration = argRuleConfiguration[i];
        int indexOfSeparator = configuration.find_last_of("=");
        std::string key = configuration.substr(0, indexOfSeparator);
        std::string value = configuration.substr(indexOfSeparator + 1,
            configuration.size() - indexOfSeparator - 1);
        CAPA::RuleConfiguration::addConfiguration(key, value);
    }

    filter.enableRules(argEnabledRules.begin(), argEnabledRules.end());
    filter.disableRules(argDisabledRules.begin(), argDisabledRules.end());
}

std::string CAPA::option::workingPath()
{
    return absoluteWorkingPath;
}

std::string CAPA::option::installPrefix()
{
    return binPath() + "/..";
}

std::string CAPA::option::binPath()
{
    return executablePath;
}

std::string CAPA::option::libPath()
{
    return installPrefix() + "/lib";
}

std::string CAPA::option::etcPath()
{
    return installPrefix() + "/etc";
}

std::string CAPA::option::homePath()
{
    const char *home = getenv("HOME");
    return home ? std::string(home) : std::string();
}

std::vector<std::string> CAPA::option::rulesPath()
{
    if (argRulesPath.size() > 0)
    {
        return argRulesPath;
    }
    std::string defaultRulePath = libPath() + "/CAPA/rules";
    std::vector<std::string> defaultRulesPath { defaultRulePath };
    return defaultRulesPath;
}

std::string CAPA::option::reporterPath()
{
    return libPath() + "/CAPA/reporters";
}

bool CAPA::option::hasOutputPath()
{
    return argOutput != "-";
}

std::string CAPA::option::outputPath()
{
    return argOutput.at(0) == '/' ? argOutput : workingPath() + "/" + argOutput;
}

std::string CAPA::option::reportType()
{
    return argReportType;
}

const CAPA::RulesetFilter &CAPA::option::rulesetFilter()
{
    return filter;
}

int CAPA::option::maxP1()
{
    return argMaxP1;
}

int CAPA::option::maxP2()
{
    return argMaxP2;
}

int CAPA::option::maxP3()
{
    return argMaxP3;
}

bool CAPA::option::showEnabledRules()
{
    return argListEnabledRules;
}

bool CAPA::option::enableGlobalAnalysis()
{
    return argGlobalAnalysis;
}

bool CAPA::option::enableClangChecker()
{
    return argClangChecker;
}

bool CAPA::option::allowDuplicatedViolations()
{
    return argDuplications;
}

bool CAPA::option::enableVerbose()
{
    return argEnableVerbose;
}
