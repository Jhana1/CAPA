#include "CAPA/Violation.h"

#include <utility>
#include <iostream>

#include "CAPA/RuleBase.h"

using namespace CAPA;

Violation::Violation(RuleBase* violatedRule, std::string violationFilePath,
                     int violationStartLine, int violationStartColumn,
                     int violationEndLine, int violationEndColumn,
                     const PatternInfo &violationPatternInfo,
                     const std::string violationMessage)
    : path(std::move(violationFilePath)), message(std::move(violationMessage)), patternInfo(violationPatternInfo)
{
    rule = violatedRule;
    startLine = violationStartLine;
    startColumn = violationStartColumn;
    endLine = violationEndLine;
    endColumn = violationEndColumn;
}

bool Violation::operator==(const CAPA::Violation &rhs) const
{
    return (path == rhs.path)
            && (startLine == rhs.startLine)
            && (startColumn == rhs.startColumn)
            && (endLine == rhs.endLine)
            && (endColumn == rhs.endColumn);
}
bool Violation::operator>(const CAPA::Violation &rhs) const
{
    if (startLine == rhs.startLine)
    {
        return patternInfo.priority > rhs.patternInfo.priority;
    }
    return startLine > rhs.startLine;
}
