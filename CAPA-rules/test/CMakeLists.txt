ADD_LIBRARY(test_helper headers/TestRuleOnCode.cpp)
IF (MINGW)
    SET(TEST_LIBS
        test_helper
        gmock
        gmock_main
        ${PROFILE_RT_LIBS}
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
        CAPAAbstractRule
        CAPACore
        CAPARuleSet
    )
ELSE()
    SET(TEST_LIBS
        test_helper
        CAPAAbstractRule
        CAPAHelper
        CAPAUtil
        CAPACore
        CAPARuleSet
        gmock
        gmock_main
        ${PROFILE_RT_LIBS}
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
    )
ENDIF()
MACRO(build_test name)
    ADD_EXECUTABLE(${name} ${ARGN})
    TARGET_LINK_LIBRARIES(${name} ${TEST_LIBS})
    ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
ENDMACRO(build_test)

BUILD_TEST(CanaryTest CanaryTest.cpp)
IF (MINGW)
    # Do it once, instead of copying for each test
    file(TO_CMAKE_PATH "${CAPA_BUILD_DIR}" CAPA_BUILD_CMAKE_PATH)
    add_custom_command(TARGET CanaryTest PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CAPA_BUILD_CMAKE_PATH}/bin/libCAPARuleSet.dll" $<TARGET_FILE_DIR:CanaryTest>)
ENDIF()

#ADD_SUBDIRECTORY(parallel)
