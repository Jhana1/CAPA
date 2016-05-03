#ifndef CAPA_DIAGNOSTICDISPATCHER_H
#define CAPA_DIAGNOSTICDISPATCHER_H

#include <clang/Basic/Diagnostic.h>

namespace CAPA
{

class DiagnosticDispatcher : public clang::DiagnosticConsumer
{
private:
    bool _isCheckerCustomer;

public:
    explicit DiagnosticDispatcher(bool runClangChecker);
    void HandleDiagnostic(clang::DiagnosticsEngine::Level diagnosticLevel,
                          const clang::Diagnostic& diagnosticInfo) override;
};

} // end namespace CAPA

#endif
