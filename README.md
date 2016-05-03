# CAPA - C Algorithm Parallelisation Analyser 

CAPA is a static code analysis tool for identifying possibly parallel algorithm implementations
within your C codebase that may be re-implemented on GPU hardware.

It looks for potential speedups including:

* Map Operations - Neighbour Invariant 1->1 transformations
* Reduce Operations - Highly Efficient N->1 transformations
* Scan (Prefix Sum) - N->N transformations 
* Matrix Multiplication - Quadratic complexity with superior GPU implementations
* Graph Traversals - Significant speedups in common depth-first traversalst
* ...
