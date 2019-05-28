---
title: PS6 Questions
---

### PS6 Written Questions

#### Norm

**Q**
Which combination of problem size, blocking, and synchronization shows the best performance?  Which shows the worst?  Explain, using what you know about threads, locking, OpenMP, computer architecture, etc.

#### Sparse Matrix-Vector Product
**Q**
Which matrix formats (if any) need to be protected against race conditions?
What approach did you use to ensure safety?  Did that approach affect performance?

**Q**
(Extra Credit)
If there is a potential for a race condition in any of the sparse matrix formats, can you create some combination of number of threads, problem size, matrix data, etc to expose it?

**Q**
Which combination of matrix format, problem size, blocking, and synchronization shows the best performance?  Which shows the worst?  Explain, using what you know about threads, locking, OpenMP, computer architecture, etc.

#### Partitioning and Load Balancing
**Q**
Which scheduling approach (and which block size) give the best load balancing performance?

#### PageRank
**Q**
What do you need to change to effect parallelization of the matrix-vector product portion of the pagerank computation?  What parallelization approach did you choose?

**Q**
(Extra Credit)
Use a parallelization approach of your choosing and report on the benefits obtained.

**Q**
(Extra Credit)
Are there any operations in the program where Amdahl's Law would limit scalability?

#### About PS6
**Q**
The most important thing I learned from this assignment was ...

**Q**
One thing I am still not clear on is ...
