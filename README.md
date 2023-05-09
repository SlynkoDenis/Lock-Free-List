# Lock-Free List

Lock-free list with set-like semantics [[1]](#1). Provided with TLA+ model and its C++ implementation with unit-tests.

## How to build

NB! [Spot](https://spot.lre.epita.fr/install.html) third-party dependency is not installed automatically, you must do it manually.

```bash
$ mkdir build && cd build
$ cmake path/to/sources -GNinja -DCMAKE_BUILD_TYPE=Debug -DSPOT_LIB_PATH=path/to/libspot.a -DBUDDY_LIB_PATH=path/to/libbddx.a
$ ninja -j4
```

## Model

Model is written in TLA+ and verified with _TLC Model Checker_. It describes _Insert_ and _Delete_ methods of the lock-free list and mainly checks that list remains connected and sorted.

## Implementation

Main features:
* Structure of _<pointer; boolean flag_> was used instead of hazard pointers
* Relies on external memory resource manager to release allocated memory after `LFList` instance goes out of scope. Unit tests use `std::synchronized_pool_resource` resource
* Unit tests checking several scenarios with various number of inserting and deleting threads

## Observer Automaton Verification 

Büchi automata were used to verify correct `Insert` and `Remove` methods operation. Liveness LTL for the methods is the following:
* Insert == Search U (AlreadyExists | SucceededCAS)
* Remove == Search U (CurrIsSentinel | (NextIsUnmarked & SucceededCAS))

Verification is done during runtime when builded in `Debug` mode by evaluating predicates corresponding to the edges. When no transitions from current automaton state exist, error is reported and execution halts.

## References
<a id="1">[1]</a> 
Timothy L. Harris. (2001).
A Pragmatic Implementation of Non-Blocking Linked-Lists. International Symposium on Distributed Computing.
