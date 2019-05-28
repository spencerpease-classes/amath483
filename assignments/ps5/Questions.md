---
title: PS5 Questions
---

### PS5 Written Questions

#### Norm!

**Q** At what problem size do the answers between the computed norms start to
differ?

Measurable differences start to occur between the norms for `Vectors` with a
size of approximately 15 (with an absolute difference on the order of 1e-16).
Trying to find this lower bound, I noticed that sometimes larger values of `N`
would show no measurable difference for one or the other sorted Vectors. This
behavior  disappeared around when `N` was on the order of 1e3.


**Q** How do the absolute and relative errors change as a function of problem
size?

As the problem size increases, so do the absolute and relative errors with a
roughly linear pattern. For both the sorted and reverse sorted vector, absolute
error increased faster than relative error. At `N = 1000000000`, absolute error
was on the order of 1e-09, and relative error was on the order of 1e-13.


**Q** Does the `Vector` class behave strictly like a member of an  abstract
**vector** class?

No, the difference in norms between ~`Vectors` that only differ in the oder of
their elements show that `Vectors` do not have the associative addition property
of an abstract vector.


**Q** Do you have any concerns about this kind of behavior?

While the difference in norms for these tests are so minuscule as to not affect
the norm until the ~10th decimal place, this could become a problem for larger
calculations. Errors will propagate throughout a problem, eventually becoming
significant if a calculation is complex enough.


#### Norm! (Partition Edition)

**Q** At what number of threads do you start to see a difference between `norm4`
and `norm3`?  Note that you can run as many threads as you like (recall the
difference between concurrency and parallelism).

Starting with two threads, there is a measurable difference between `norm3` and
`norm4`.


**Q** Is this difference always the same?

For a given `N` the absolute and relative differences appear to stay on the same
order of magnitude between 2 and 1000 threads. For example, with `N = 10000000`,
absolute error was on the order of 1e-11, while relative error was on the order
of 1e-14.


**Q** Do you have any concerns about this kind of behavior?

Its important to note that running a problem on multiple threads does add some
small error. This means that large problems will be less accurate if you
partition the problem on multiple threads for increased speed. However, its good
to know that once you take the initial accuracy impact of partitioning, you
don't lose significant accuracy for running on more threads.


#### Norm! (Recursion Version)

**Q** Was there any appreciable difference between the partitioned and the
recursive versions?

From 1 to 4 workers (my CPU has 4 physical cores), there appears to be
little difference between the partitioned and recursive norm, with the recursive
norm having a small edge. Between 8 and 10 workers, however, the differences
decrease, and eventually, the partitioned norm gains an edge.

#### About PS5


**Q** The most important thing I learned from this assignment was ...

You cannot declare methods within other methods, you need to use function
objects (this was important for recursion).


**Q** One thing I am still not clear on is ...

In practice, what is the difference between `std::thread` and `std::async`?
