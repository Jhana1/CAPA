#include "CAPA/DiagnosticDispatcher.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>

#include "CAPA/ResultCollector.h"
#include "CAPA/Violation.h"

using namespace CAPA;

DiagnosticDispatcher::DiagnosticDispatcher(bool runClangChecker)
{
    _isCheckerCustomer = runClangChecker;
}

void DiagnosticDispatcher::HandleDiagnostic(clang::DiagnosticsEngine::Level diagnosticLevel,
    const clang::Diagnostic &diagnosticInfo)
{
    clang::DiagnosticConsumer::HandleDiagnostic(diagnosticLevel, diagnosticInfo);

    clang::SourceLocation location = diagnosticInfo.getLocation();
    clang::SourceManager *sourceManager = &diagnosticInfo.getSourceManager();
    llvm::StringRef filename = sourceManager->getFilename(location);
    int line = sourceManager->getPresumedLineNumber(location);
    int column = sourceManager->getPresumedColumnNumber(location);

    clang::SmallString<100> diagnosticMessage;
    diagnosticInfo.FormatDiagnostic(diagnosticMessage);

    PatternInfo p;
    Violation violation(nullptr, filename.str(), line, column, 0, 0, p,
                        diagnosticMessage.str().str());

    ResultCollector *results = ResultCollector::getInstance();
    if (_isCheckerCustomer)
    {
        results->addCheckerBug(violation);
    }
    else
    {
        if (diagnosticLevel == clang::DiagnosticsEngine::Warning)
        {
            results->addWarning(violation);
        }
        if (diagnosticLevel == clang::DiagnosticsEngine::Error ||
            diagnosticLevel == clang::DiagnosticsEngine::Fatal)
        {
            results->addError(violation);
        }
    }
}
