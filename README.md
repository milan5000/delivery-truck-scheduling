# delivery-truck-scheduling
Computational simulation of delivery truck scheduling models in C++.

This project is based on the delivery truck scheduling project from Chapter 52 of [Introduction to Scientific Programming in C++17/Fortran2008](https://theartofhpc.com/) by Victor Eijkhout. It constructs a framework for assembling delivery truck routes and then solving the Traveling Salesman Problem (TSP) over them, both with the naive greedy algorithm and with the [2-opt algorithm](https://en.wikipedia.org/wiki/2-opt).

Implementation is done in C++11. Routes may be constructed using the `Route` class, which contains an ordered sequence of `Address` objects to deliver to, then various improvements may be made via `greedy_route()` or `opt2_rearrange()`. `Route` preserves the starting and ending locations to simulate depots; the alternative `AddressList` class may be used to avoid this functionality. All objects support the use of both Euclidean and Manhattan (taxicab) distance.

Test code and example implementations are available in `tester.cpp`. Running programs may be done in `main.cpp`, though the file is currently unused.

This project may or may not be revisited in the future; possible next steps for this project include solution of the multiple TSP for multiple delivery trucks, the use of delivery deadlines to create scenarios that evolve over time, implementation of other heuristics such as furthest point insertion or 3-opt tours, or visualization of delivery routes.

This is the first "serious" project I have done with C++, mainly done for learning purposes, so any contribution, feedback, feature requests, or issues are greatly appreciated.
