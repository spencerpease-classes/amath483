Sample: reduction
Minimum spec: SM 3.0

A parallel sum reduction that computes the sum of a large arrays of values.  This sample demonstrates several important optimization strategies for 1:Data-Parallel Algorithms like reduction.

Key concepts:
Data-Parallel Algorithms
Performance Strategies

Command line arguments:
    "--shmoo":         Test performance for 1 to 32M elements with each of the 7 different kernels
    "--n=<N>":         Specify the number of elements to reduce (default 1048576)
    "--threads=<N>":   Specify the number of threads per block (default 128)
    "--kernel=<N>":    Specify which kernel to run (0-6, default 6)
    "--maxblocks=<N>": Specify the maximum number of thread blocks to launch (kernel 6 only, default 64)
    "--cpufinal":      Read back the per-block results and do final sum of block sums on CPU (default false)
    "--cputhresh=<N>": The threshold of number of blocks sums below which to perform a CPU final reduction (default 1)
    "-type=<T>":       The datatype for the reduction, where T is "int", "float", or "double" (default int)
