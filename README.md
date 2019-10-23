# Homework-2-Data-Structures-Uber-Simulator
Helped me to structure a project, to improve coding style and implement generic containers data structures

Nedelcu Horia
311CA Group


Homework2 - Data Structures - Uber Simulator

  * Uber Simulator Problem:
  We modeled the problem with a graph that stores as a data for each node
a string associated with each node's name. To make a quick conversion from
the name of intersections and drivers to their indexes we used 2 hashes. We
also use a distance-matrix, which we pre-calculate at task 3, so we don't have
to call BFS for each query at task 4 (Complexity reduced from O((V+E)*Q) to
O((V+E)^2+Q), V+E << Q).

  * Hashtable Implementation:
  Considering the good injective hash function, an efficient caching
implementation is the open addressing method. Also, for the performance of
the running time, we choose to delete in lazy fashion, marking the slots
deleted whene we remove a key.

  * Graph Implementation:
  Dealing with sparse graph (V >> E), we choose to store it with adjacency
list to increase time performance of BFS (O(V+E) != O(V^2) <= adjacency matrix).
Also each node keeps associated information, in our case we have strings with
names of the intersections.

  * Sorted List Implementation:
  The Drivers' rankings are stored using sorted lists in which the deletion
and insertion are done in O(N) time. To increase performance, we can replace
this structure with an AVL, Red-Black Tree, SkipList or other data structures
with the upper O(logN) time limit.
