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

SpanningForest prim(int n, const std::vector<Edge> & bridges) {
  
  SpanningForest forest;
  const int m = static_cast<int>(bridges.size());

  //Adjacency list in compressed (CSR) form
  //x are adj[start[x]] .. adj[start[x + 1] - 1], stored as bridge indicess
  std::vector<int> start(static_cast<std::size_t>(n) + 1, 0);
  for (const Edge &e : bridges)
  {
    ++start[e.from + 1];
    ++start[e.to + 1];
  }

  for (int x = 0; x < n; ++x) 
  {
    start[x + 1] += start[x];
  }

  std::vector<int> adj(2 * static_cast<std::size_t>(m));
  std::vector<int> next(start.begin(), start.end() - 1);


  for (int i = 0; i < m; ++i) 
  {
    adj[next[bridges[i].from]++] = i;
    adj[next[bridges[i].to]++] = i;
  }

  //Heap entry: (length, bridge index, island it leads to). Min-heap order
  //by length, then bridge index
  using Entry = std::tuple<std::int64_t, int, int>;
  std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> heap;
  std::vector<char> in_tree(n, 0);

  auto add_island = [&](int x) 
  {
    in_tree[x] = 1;
    for (int k = start[x]; k < start[x + 1]; ++k)
    {

      const Edge &e = bridges[adj[k]];

      int other = e.from == x ? e.to : e.from;

      if (!in_tree[other]) heap.emplace(e.weight, adj[k], other);

    }
  };

  // Restart in every region: each island not yet reached roots a new tree.
  for (int root = 0; root < n; ++root) 
  {
    if (in_tree[root]) continue;

    add_island(root);


    while (!heap.empty()) 
    {
      auto [length, i, x] = heap.top();
      heap.pop();
      if (in_tree[x]) continue;  // Stale
      forest.total_length += length;
      forest.edges.push_back(bridges[i]);
      add_island(x);
    }
  }
  return forest;
}
