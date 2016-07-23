#ifndef CAPA_VIOLATION_H
#define CAPA_VIOLATION_H

#include <string>
#include "CAPA/PatternInfo.h"

namespace CAPA
{

class RuleBase;

class Violation
{
public:
    const RuleBase *rule;
    std::string path;
    int startLine;
    int startColumn;
    int endLine;
    int endColumn;
    std::string message;
    PatternInfo patternInfo;

    Violation(RuleBase* violatedRule, std::string violationFilePath,
              int violationStartLine, int violationStartColumn,
              int violationEndLine, int violationEndColumn, 
              const PatternInfo &violationPatternInfo,
              const std::string violationMessage = "");

    bool operator==(const CAPA::Violation &rhs) const;
};

} // end namespace CAPA

#endif
