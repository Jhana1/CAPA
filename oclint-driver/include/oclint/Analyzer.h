#ifndef CAPA_ANALYZER_H
#define CAPA_ANALYZER_H

#include <vector>

namespace clang
{
    class ASTContext;
}

namespace CAPA
{

class Analyzer
{
public:
    virtual void preprocess(std::vector<clang::ASTContext *> &contexts) {}
    virtual void analyze(std::vector<clang::ASTContext *> &contexts) = 0;
    virtual void postprocess(std::vector<clang::ASTContext *> &contexts) {}
};

} // end namespace CAPA

#endif
