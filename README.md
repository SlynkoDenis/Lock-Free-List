# Lock-Free List

Lock-free list with set-like semantics [[1]](#1). Provided with TLA+ model and its C++ implementation with unit-tests.

## Model

Model is written in TLA+ and verified with _TLC Model Checker_. It describes _Insert_ and _Delete_ methods of the lock-free list and mainly checks that list remains connected and sorted.

## Implementation

Main features:
* Structure of _<pointer; boolean flag_> was used instead of hazard pointers
* Relies on external memory resource manager to release allocated memory after `LFList` instance goes out of scope. Unit tests use `std::synchronized_pool_resource` resource
* Unit tests checking several scenarios with various number of inserting and deleting threads

## References
<a id="1">[1]</a> 
Timothy L. Harris. (2001).
A Pragmatic Implementation of Non-Blocking Linked-Lists. International Symposium on Distributed Computing.
