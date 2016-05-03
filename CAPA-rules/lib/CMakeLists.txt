ADD_SUBDIRECTORY(helper)
ADD_SUBDIRECTORY(util)

SET (AbstractSource
    AbstractASTMatcherRule.cpp
    AbstractASTRuleBase.cpp
    AbstractSourceCodeReaderRule.cpp
)

IF (MINGW)
    ADD_LIBRARY(CAPAAbstractRule SHARED
        ${AbstractSource}
        helper/AttributeHelper.cpp
        helper/EnforceHelper.cpp
        helper/SuppressHelper.cpp
        util/ASTUtil.cpp
        util/StdUtil.cpp
        util/MyASTUtil.cpp)
    TARGET_LINK_LIBRARIES(CAPAAbstractRule
        CAPACore
        CAPARuleSet
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
    )
    SET_TARGET_PROPERTIES(CAPAAbstractRule
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    )

    TARGET_LINK_LIBRARIES(CAPAAbstractRule LINK_INTERFACE_LIBRARIES CAPARuleSet)
ELSE()
    ADD_LIBRARY(CAPAAbstractRule ${AbstractSource})
ENDIF()
