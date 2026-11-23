//Minimum spanning forests of the bridge network: Kruskal's algorithm with a disjoint-set forest
//Prim's algorithm with a binary heap restarted in every region

#include "maintenance.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <numeric>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

#include "disjoint_set.h"

//O(m log m) for the sort plus O(m o(n)) for the disjoint-set operations,
//so O(m log n) overall
SpanningForest kruskal(int n, const std::vector<Edge> &bridges) {

  SpanningForest forest;

  const int m = static_cast<int>(bridges.size());

  //Sort indices rather than copying edges
  std::vector<int> order(m);

  std::iota(order.begin(), order.end(), 0);

  std::sort(order.begin(), order.end(), [&](int a, int b)
  {
    return std::make_pair(bridges[a].weight, a) <
           std::make_pair(bridges[b].weight, b);
  });

  DisjointSet sets(n);

  for (int i : order) 
  {
    const Edge &e = bridges[i];
    
    if (sets.unite(e.from, e.to)) 
    {
      forest.total_length += e.weight;
      forest.edges.push_back(e);
      // A forest on n islands has at most n - 1 bridges
      if (static_cast<int>(forest.edges.size()) == n - 1) break;

    }
  }
  
  return forest;
}

SpanningForest prim(int /*n*/, const std::vector<Edge> & /*bridges*/) {
  return {};  // TODO
}
