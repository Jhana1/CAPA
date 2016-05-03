
SET(BuildTest1Rule Test1)
BUILD_DYNAMIC_RULES("${BuildTest1Rule}")

SET(BuildNothingRule Nothing)
BUILD_DYNAMIC_RULES("${BuildNothingRule}")
