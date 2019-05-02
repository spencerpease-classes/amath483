---
title: PS4 Questions
---

### PS4 Written Questions

Add your answers to this file in plain text after each question.  Leave a blank line between the text of the question and the text of your answer.


#### Ordering matters


**Q**
There should be some differences between the results for the three functions for multiplying matrices (ijk, ikj, jki).  Using what you know about memory hierarchy, CPU operation, and data layout for row-major matrices, explain the behavior you are seeing.  (Hint: The three functions differ in the variable they iterate over in their inner loops.)



### Orientation

**Q**
For each of the ijk, ikj, and jki orderings for a row-oriented matrix-matrix product, what are the equivalent orderings for a column-oriented matrix-matrix product?



#### Does ordering matter?

**Q** Again, using what you
know about memory hierarchy, SIMD/vectorization, CPU operation, and data
layout for row-major matrices, explain the behavior you
are seeing with `matvec_ij` and `matvec_ji`.


#### Vector of Vectors

**Q**
Explain any differences between what you see for `VoVMatrix` and what you see for `Matrix`



**Q**
Does `VoVMatrix` respond in the same way for different loop orderings as does `Matrix`?



#### About PS4


**Q** The most important thing I learned from this assignment was ...


**Q** One thing I am still not clear on is ...
