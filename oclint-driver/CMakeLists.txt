CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(CAPA_DRIVER)

SET(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/../CAPA-core/cmake"
    )

INCLUDE(OCLintConfig)

INCLUDE_DIRECTORIES(
    ${CAPA_SOURCE_DIR}/include
    )
LINK_DIRECTORIES(
    ${CAPA_BUILD_DIR}/lib
    )

INCLUDE_DIRECTORIES(${CAPA_DRIVER_SOURCE_DIR}/include)

ADD_SUBDIRECTORY(lib)

IF(MINGW)
    ADD_EXECUTABLE(CAPA-${CAPA_VERSION_RELEASE}
        main.cpp
        reporters_windows_port.cpp
        rules_windows_port.cpp
        )
ELSE()
    ADD_EXECUTABLE(CAPA-${CAPA_VERSION_RELEASE}
        main.cpp
        reporters_dlfcn_port.cpp
        rules_dlfcn_port.cpp
        )
ENDIF()

TARGET_LINK_LIBRARIES(CAPA-${CAPA_VERSION_RELEASE}
    OCLintDriver
    OCLintRuleSet
    OCLintCore
    clangStaticAnalyzerFrontend
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangRewriteFrontend
    clangRewrite
    ${CLANG_LIBRARIES}
    ${REQ_LLVM_LIBRARIES}
    ${CMAKE_DL_LIBS}
    )
