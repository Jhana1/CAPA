#ifndef CAPA_COMPILERINSTANCE_H
#define CAPA_COMPILERINSTANCE_H

#include <vector>

#include <clang/Frontend/CompilerInstance.h>

namespace clang
{
    class FrontendAction;
}

namespace CAPA
{

class CompilerInstance : public clang::CompilerInstance
{
public:
    void start();
    void end();

private:
    std::vector<clang::FrontendAction *> _actions;
};

} // end namespace CAPA

#endif
