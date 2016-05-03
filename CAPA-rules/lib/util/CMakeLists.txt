IF (NOT MINGW)
    ADD_LIBRARY(CAPAUtil
        ASTUtil.cpp
        StdUtil.cpp
    )
ENDIF()
