
SET(BuildMapRule Map)
BUILD_DYNAMIC_RULES("${BuildMapRule}")

SET(BuildReduceRule Reduce)
BUILD_DYNAMIC_RULES("${BuildReduceRule}")

SET(BuildScanRule Scan)
BUILD_DYNAMIC_RULES("${BuildScanRule}")

SET(BuildConstantFoldRule ConstantFold)
BUILD_DYNAMIC_RULES("${BuildConstantFoldRule}")

SET(BuildMatrixMultRule MatrixMult)
BUILD_DYNAMIC_RULES("${BuildMatrixMultRule}")

SET(BuildVectorisableRule Vectorisable)
BUILD_DYNAMIC_RULES("${BuildVectorisableRule}")

SET(BuildVectorFunctionDeclRule VectorFunctionDecl)
BUILD_DYNAMIC_RULES("${BuildVectorFunctionDeclRule}")
