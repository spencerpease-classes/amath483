---
title: Midterm Questions
---

### Midterm Written Questions

Add your answers to this file in plain text after each question.  Leave a blank
line between the text of the question and the text of your answer.


#### Sparse-matrix dense-matrix product


**Q**
How does the performance (in GFLOP/s) for sparse-matrix by dense matrix product
(**SPMM**) compare  to sparse-matrix by vector product (**SPMV**)? The
performance for SPMM should be about the same as for SPMV in the case of a 1
column dense matrix.   What is the trend with increasing numbers of columns?

For **SPMV**, the performance remains relatively constant across all matrix
sizes (between approx. 1 and 10 Gflop/s). This performance matches the **SPMM**
performance for a one-column rhs dense matrix.

As the number of rhs columns increases in **SPMM** however, performance begins
to drop across all implementations for increasing matrix size, with COO taking
the worst hit. This is likely due to the need of **SPMM** to still loop over
the columns of the dense rhs matrix, meaning computation increases with each
column.


**Q**
How does the performance of SPMM (in GFLOP/s) compare to the results you got
dense matrix-matrix product in previous assignments?   There should be a fairly
large difference in GFLOP/s between sparse and dense matrix methods.  Give some
reasons for such a big difference.

**SPMM** performance is much lower than **matrix-matrix** performance, with
**matrix-matrix** actually increasing in performance with matrix size to a
certian point. This is likely due to the optomizations the compiler is able to
make for **matrix-matrix** multiplication (ie hoisting, predicting data needs),
while **SPMM** is limited to needing to access more arrays, as well as access
those arrays in linear (instead of constant) time.


#### Compressed sparse column

**Q**
How does the performance of SPMM (in GFLOP/s) compare to the results you got
dense matrix-matrix product in previous assignments?   There should be a fairly
large difference in GFLOP/s between sparse and dense matrix methods.  Give some
reasons for such a big difference.

Again, the dense **matrix-matrix** operation is much more performant than
**SPMM**. This is likely due to a sparce SCS matrix needing linear time to
access its internal arrays to get data, whereas a dense matrix can access an
index in constant time.


**Q**
How does the performance of CSC compare to the performance of CSR and of COO?
Explain why (or why not) there are any significant differences.




#### Array of structs

**Q**
How does the performance of AOS compare to the performance of  COO?   Explain
why (or why not) there are any significant differences.

