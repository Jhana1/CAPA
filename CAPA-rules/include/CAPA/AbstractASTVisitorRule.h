#ifndef CAPA_ABSTRACTASTVISITORRULE_H
#define CAPA_ABSTRACTASTVISITORRULE_H

#include <clang/AST/RecursiveASTVisitor.h>

#include "CAPA/AbstractASTRuleBase.h"

namespace CAPA
{

template<typename T>
class AbstractASTVisitorRule : public AbstractASTRuleBase, protected clang::RecursiveASTVisitor<T>
{
    friend class clang::RecursiveASTVisitor<T>;
protected:
    virtual void apply()
    {
        if (!isLanguageSupported())
        {
            return;
        }

        setUp();
        clang::SourceManager *sourceManager = &_carrier->getSourceManager();
        clang::DeclContext *decl = _carrier->getTranslationUnitDecl();
        for (clang::DeclContext::decl_iterator it = decl->decls_begin(), declEnd = decl->decls_end();
            it != declEnd; ++it)
        {
            clang::SourceLocation startLocation = (*it)->getLocStart();
            if (startLocation.isValid() &&
                sourceManager->getMainFileID() == sourceManager->getFileID(startLocation))
            {
                (void) /* explicitly ignore the return of this function */
                    clang::RecursiveASTVisitor<T>::TraverseDecl(*it);
            }
        }
        tearDown();
    }

public:
    virtual ~AbstractASTVisitorRule() {}

    virtual void setUp() {}
    virtual void tearDown() {}
};

} // end namespace CAPA

#endif
