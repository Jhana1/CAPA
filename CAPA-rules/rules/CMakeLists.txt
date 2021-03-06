MACRO(build_dynamic_rule name)
    IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        SET(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
    ENDIF()

    ADD_LIBRARY(${name}Rule SHARED ${name}Rule.cpp)

    SET_TARGET_PROPERTIES(${name}Rule
        PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/rules.dl
                   RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/rules.dl
        )

    IF (MINGW)
        TARGET_LINK_LIBRARIES(${name}Rule
            CAPAAbstractRule
        )
    ELSE()
        TARGET_LINK_LIBRARIES(${name}Rule CAPAAbstractRule)

        TARGET_LINK_LIBRARIES(${name}Rule
            clangASTMatchers
        ) # TODO: might be redundant

        TARGET_LINK_LIBRARIES(${name}Rule
            CAPAHelper
            CAPAUtil
            CAPACore
        )

        IF (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
            TARGET_LINK_LIBRARIES(${name}Rule
                ${CLANG_LIBRARIES}
                ${REQ_LLVM_LIBRARIES}
                CAPARuleSet
            )
        ENDIF()
    ENDIF()

ENDMACRO(build_dynamic_rule)

MACRO(build_dynamic_rules rules)
    FOREACH(it ${rules})
        BUILD_DYNAMIC_RULE(${it})
    ENDFOREACH(it)
ENDMACRO(build_dynamic_rules)

MACRO(add_rule_category_directory category)
    IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${category}/CMakeLists.txt)
        ADD_SUBDIRECTORY(${category})
    ENDIF()
ENDMACRO(add_rule_category_directory)

ADD_RULE_CATEGORY_DIRECTORY(parallel)
