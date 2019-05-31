---
title: PS6 Questions
---

### PS6 Written Questions

#### Norm

**Q** Which combination of problem size, blocking, and synchronization shows the
best performance?  Which shows the worst?  Explain, using what you know about
threads, locking, OpenMP, computer architecture, etc.

Of the parallel implementations of `norm`, block reduction showed the best
performance, though it wasn't always faster than the sequential norm. Adding
critical (or atomic) sections to `norm` led to the worst performance, which
makes sense because doing that essentially reduces you down to sequential
operations, but with extra overhead. Using reduction instead produced safe and
faster results.

Using blocking instead of cyclic parallelization also led to better performance,
likely because with blocking, the processor can make use of locality to predict
the data a thread will request next, since it access the vector in order.

Parallel implementations also performed best for large problem sizes. This is
likely because there is some overhead with creating threads, so its necessary to
have enough work for each thread to make up for the initial cost. For the small
problem, it was faster to do everything sequentially, because the problem was
small enough.

#### Sparse Matrix-Vector Product

**Q** Which matrix formats (if any) need to be protected against race
conditions? What approach did you use to ensure safety?  Did that approach
affect performance?

None of the matrix formats is did (CSR, COO) needed to be protected from race
conditions when using open mp. Since the fundamental operation in these
`matvec` function is to update an index in a vector, as long as you tell omp
that the vector needs to be shared across threads, it takes care of it.

This approach led to an increase in performance for more threads to a point, and
then a decrease.

**Q** (Extra Credit) If there is a potential for a race condition in any of the
sparse matrix formats, can you create some combination of number of threads,
problem size, matrix data, etc to expose it?

**Q** Which combination of matrix format, problem size, blocking, and
synchronization shows the best performance?  Which shows the worst?  Explain,
using what you know about threads, locking, OpenMP, computer architecture,
etc.


#### Partitioning and Load Balancing

**Q** Which scheduling approach (and which block size) give the best load
**balancing performance?


#### PageRank

**Q** What do you need to change to effect parallelization of the matrix-vector
product portion of the pagerank computation?  What parallelization approach
did you choose?

I used the `omp_matvec()` function to do the matrix-vector product in parallel
instead of the default `operator*()`. This make use of the `omp parallel for`
directive in open mp to handle breaking the problem into threads.


**Q** (Extra Credit) Use a parallelization approach of your choosing and report
on the benefits obtained.

**Q** (Extra Credit) Are there any operations in the program where Amdahl's Law
**would limit scalability?


#### About PS6

**Q** The most important thing I learned from this assignment was ...

Parallelization isn't always a one-size-fits-all for speed improvements.

**Q** One thing I am still not clear on is ...

How do you do partition by non zeros? Also, why does the sequential norm
consistently perform so well against the parallel versions. Specifically,
I thought using cyclic reduction would be the fastest, but it ends up being
pretty slow.
