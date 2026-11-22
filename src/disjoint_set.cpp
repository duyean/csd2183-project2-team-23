// Disjoint-set forest with union by rank and path compression
// (Cormen et al., Section 19.3).

#include "disjoint_set.h"

#include <utility>

DisjointSet::DisjointSet(int n) : parent_(n), rank_(n, 0), count_(n) {
  //Every element starts as the root of its own one-node tree.

  for (int i = 0; i < n; ++i) 
  {
    parent_[i] = i;
  }
  
}

int DisjointSet::find(int x) {
  // First pass
  int root = x;

  while (parent_[root] != root) 
  {
    root = parent_[root];
  }

  //Second pass
  while (parent_[x] != root) 
  {
    int next = parent_[x];
    parent_[x] = root;
    x = next;
  }

  return root;
}

bool DisjointSet::unite(int x, int y) {

  int rx = find(x);
  int ry = find(y);

  if (rx == ry) 
  {
    return false;
  }

  //Attach the root of lower rank below the root of higher
  if (rank_[rx] < rank_[ry]) 
  {
    std::swap(rx, ry);
  }

  parent_[ry] = rx;

  if (rank_[rx] == rank_[ry]) 
  {
    ++rank_[rx];
  }

  --count_;
  return true;

}

int DisjointSet::count() const {
  return count_;
}
