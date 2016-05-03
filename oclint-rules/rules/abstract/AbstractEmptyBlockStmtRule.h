#ifndef CAPA_ABSTRACTEMPTYBLOCKSTMTRULE_H
#define CAPA_ABSTRACTEMPTYBLOCKSTMTRULE_H

template <typename T>
class AbstractEmptyBlockStmtRule : public CAPA::AbstractASTVisitorRule<T>
{
protected:
    bool isLexicalEmpty(clang::Stmt *stmt)
    {
        clang::CompoundStmt *compoundStmt = clang::dyn_cast_or_null<clang::CompoundStmt>(stmt);
        return clang::isa<clang::NullStmt>(stmt) || (compoundStmt && compoundStmt->body_empty());
    }

    bool checkLexicalEmptyStmt(clang::Stmt *stmt, CAPA::RuleBase *rule)
    {
        if (stmt && isLexicalEmpty(stmt))
        {
            CAPA::AbstractASTVisitorRule<T>::addViolation(stmt, rule);
        }

        return true;
    }
};

#endif
