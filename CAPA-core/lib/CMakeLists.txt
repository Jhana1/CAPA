ADD_LIBRARY(CAPACore
    AbstractResults.cpp
    ResultCollector.cpp
    UniqueResults.cpp
    RawResults.cpp
    RuleCarrier.cpp
    Version.cpp
    Violation.cpp
    ViolationSet.cpp
    PatternInfo.cpp
    BenchmarkSet.cpp
    )

IF (${CMAKE_SYSTEM_NAME} MATCHES "Win")
ADD_LIBRARY(CAPARuleSet SHARED
    RuleConfiguration.cpp
    RuleSet.cpp
    )
  IF(TEST_BUILD)
    target_link_libraries (CAPARuleSet --coverage)
  ENDIF()
ELSE()
ADD_LIBRARY(CAPARuleSet
    RuleConfiguration.cpp
    RuleSet.cpp
    )
ENDIF()
