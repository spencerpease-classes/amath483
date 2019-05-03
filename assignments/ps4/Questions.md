--- title: PS4 Questions ---

### PS4 Written Questions

Add your answers to this file in plain text after each question.  Leave a blank
line between the text of the question and the text of your answer.


#### Ordering matters


**Q** There should be some differences between the results for the three
functions for multiplying matrices (ijk, ikj, jki).  Using what you know about
memory hierarchy, CPU operation, and data layout for row-major matrices, explain
the behavior you are seeing.  (Hint: The three functions differ in the variable
they iterate over in their inner loops.)

All computers today have a hierarchy of memory, where each tier has a different
balance of size and speed. Data stored in the CPU registers is the fastest, but
also small and expensive. From there data can move to different levels of cache,
random access memory, and finally storage, which is large and cheap, but also
many orders of magnitude slower. Because of this, high performance computation
relies on exploiting this memory hierarchy to place relevant data in quickly
accessible places. CPUs also try to increase performance by preemptively moving
data closer to the registers that it 'thinks' it will need soon, moving in data
physically close to the data it currently needs. When building an algorithm for
row-major matrix-matrix multiplication, we want to make sure the CPU's guesses
are correct by keeping temporally close data physically close too. Since the
internal structure of our matrix implemtation is a 1D array, data is ordered
sequentially by row, with every jth index signaling the start of a new row.
Therefore, when we traverse this matrix, we want to traverse through rows most
quickly, since the data in the array are next to each other and most readily
avialalbe to the CPU through it's guessing. If we were to travere though a
column of a row-oriented matrix instead, it would involve jumping aroung the
internal storage array, making more costly for the CPU to fetch data it didn't
preemptively get.

`ikj` ordering of the loops for the fundamental operation
`C(i, j) += A(i, k) * B(k, j)` in matrix-matrix multiplication proiritizes
iterating over the rows of _A_ and _B_ the quickest, which explains why it is
the most performant ordering. On the opposite end, `jki` ordering is the
slowest, because it proritizes iterating over columns, which takes more time for
a CPU to do.



### Orientation

**Q** For each of the ijk, ikj, and jki orderings for a row-oriented
matrix-matrix product, what are the equivalent orderings for a column-oriented
matrix-matrix product?

Equivalent orderings:

| Row-oriented | Column-oriented |
|--------------|-----------------|
| `ijk`        | `jik`           |
| `ikj`        | `jki`           |
| `jki`        | `ikj`           |


#### Does ordering matter?

**Q** Again, using what you know about memory hierarchy, SIMD/vectorization, CPU
operation, and data layout for row-major matrices, explain the behavior you are
seeing with `matvec_ij` and `matvec_ji`.


#### Vector of Vectors

**Q** Explain any differences between what you see for `VoVMatrix` and what you
see for `Matrix`



**Q** Does `VoVMatrix` respond in the same way for different loop orderings as
does `Matrix`?



#### About PS4


**Q** The most important thing I learned from this assignment was ...

Even though we are using optomizations when compiling, the ordering of loops in
matrix multiplication can still have an order-of-magnitude effect on
performance.


**Q** One thing I am still not clear on is ...

What exactly everything in the `Makefile` does. For example, why is the point of
`make depend`, `.PHONY`, and `PCHS`?
