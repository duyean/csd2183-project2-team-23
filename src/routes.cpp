// Your shortest-path algorithms. Every function below is a stub that compiles
// but does nothing useful yet; replace each one with your implementation.

#include "routes.h"
#include <queue>

ShortestPaths dijkstra(int n, const std::vector<Edge> &edges, int source) {
  ShortestPaths result;
  result.dist.assign(n, kUnreachable);
  result.parent.assign(n, -1);

  std::vector<std::vector<std::pair<int, std::int64_t>>> adj(n);
  for (const Edge &e : edges) {
    adj[e.from].emplace_back(e.to, e.weight);
  }

  using Item = std::pair<std::int64_t, int>;
  std::priority_queue<Item, std::vector<Item>, std::greater<Item>> heap;

  result.dist[source] = 0;
  heap.emplace(0, source);

  while (!heap.empty()) {
    auto [d, u] = heap.top();
    heap.pop();
    if (d > result.dist[u]) {
      continue;
    }
    for (const auto &[v, w] : adj[u]) {
      std::int64_t nd = d + w;
      if (nd < result.dist[v]) {
        result.dist[v] = nd;
        result.parent[v] = u;
        heap.emplace(nd, v);
      }
    }
  }

  return result;
}

BellmanFordResult bellman_ford(int /*n*/, const std::vector<Edge> & /*edges*/,
                               int /*source*/) {
  return {};  // TODO
}
