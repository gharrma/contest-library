Minimum Spanning Tree (MST)
===========================

Kruskal's Algorithm
-------------------

- Sort edges by weight.

- Use disjoint-set data structure. For each edge, if endpoints in
  different sets, add edge to tree. Merge endpoint sets.


Prim's Algorithm
----------------

- Use a min heap sorted by edge weight.

- Start with any vertex; add all adjacent edges to heap.

- While heap nonempty, find next edge with an unmarked endpoint.
  Add edge to tree and mark endpoint. Add all adjacent edges to heap.
