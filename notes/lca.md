Least Common Ancestor (LCA)
===========================

- BFS from root to mark lower nodes with lower numbers.

- Fill an array with nodes ordered by an Euler tour of the graph
  (traversal around the contour). Keep track of node locations.

- Use a segment tree (or another RMQ method) with the array above as
  leaves to find the minimum node between two query nodes.
