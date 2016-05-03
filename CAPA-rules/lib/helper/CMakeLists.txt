IF (NOT MINGW)
    ADD_LIBRARY(CAPAHelper
        AttributeHelper.cpp
        EnforceHelper.cpp
        SuppressHelper.cpp
    )
ENDIF()
