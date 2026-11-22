#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "bench_graphs.h"
#include "graph.h"
#include "routes.h"

namespace {

struct RoundCount {
  int passes;
  int relaxing_passes;
  std::vector<std::int64_t> dist;
};

RoundCount count_rounds(int n, const std::vector<Edge> &edges, int source) {
  RoundCount result{0, 0, std::vector<std::int64_t>(n, kUnreachable)};
  std::vector<std::int64_t> &dist = result.dist;
  dist[source] = 0;
  for (int round = 0; round < n; ++round) {
    bool relaxed = false;
    for (const Edge &e : edges) {
      if (dist[e.from] == kUnreachable) {
        continue;
      }
      const std::int64_t nd = dist[e.from] + e.weight;
      if (nd < dist[e.to]) {
        dist[e.to] = nd;
        relaxed = true;
      }
    }
    ++result.passes;
    if (!relaxed) {
      break;
    }
    ++result.relaxing_passes;
  }
  return result;
}

int min_hop_depth(int n, const std::vector<Edge> &edges,
                  const std::vector<std::int64_t> &dist, int source) {
  std::vector<Edge> sorted = edges;
  std::sort(sorted.begin(), sorted.end(), [&](const Edge &a, const Edge &b) {
    return dist[a.from] < dist[b.from];
  });
  const int inf = n + 1;
  std::vector<int> hops(n, inf);
  hops[source] = 0;
  for (const Edge &e : sorted) {
    if (dist[e.from] == kUnreachable || hops[e.from] == inf) {
      continue;
    }
    if (dist[e.from] + e.weight == dist[e.to]) {
      hops[e.to] = std::min(hops[e.to], hops[e.from] + 1);
    }
  }
  int best = 0;
  for (int v = 0; v < n; ++v) {
    if (hops[v] != inf) {
      best = std::max(best, hops[v]);
    }
  }
  return best;
}

void report(const std::string &family, int n, const std::vector<Edge> &edges,
            const std::string &order, int seed, int hops) {
  const RoundCount rc = count_rounds(n, edges, 0);
  std::printf("%s,%d,%zu,%s,%d,%d,%d,%d\n", family.c_str(), n, edges.size(),
              order.c_str(), seed, hops, rc.passes, rc.relaxing_passes);
}

void run_orders(const std::string &family, int n,
                const std::vector<Edge> &edges, std::mt19937_64 &rng) {
  const std::vector<std::int64_t> dist = dijkstra(n, edges, 0).dist;
  const int hops = min_hop_depth(n, edges, dist, 0);

  report(family, n, edges, "original", 0, hops);

  std::vector<Edge> ascending = edges;
  std::stable_sort(ascending.begin(), ascending.end(),
                   [&](const Edge &a, const Edge &b) {
                     return dist[a.from] < dist[b.from];
                   });
  report(family, n, ascending, "tail_dist_ascending", 0, hops);

  std::vector<Edge> descending(ascending.rbegin(), ascending.rend());
  report(family, n, descending, "tail_dist_descending", 0, hops);

  for (int seed = 1; seed <= 20; ++seed) {
    std::vector<Edge> shuffled = edges;
    std::mt19937_64 local(rng() ^ static_cast<std::uint64_t>(seed));
    std::shuffle(shuffled.begin(), shuffled.end(), local);
    report(family, n, shuffled, "random", seed, hops);
  }
}

}  // namespace

int main() {
  std::mt19937_64 rng(2183);
  std::printf("family,n,m,order,seed,max_min_hops,passes,relaxing_passes\n");
  for (const int n : {1'024, 2'048, 4'096, 8'192, 16'384}) {
    run_orders("sparse", n, random_sparse_graph(n, std::size_t{4} * n, rng), rng);
  }
  for (const int side : {32, 45, 64, 91, 128}) {
    run_orders("grid", side * side, grid_graph(side, rng), rng);
  }
}
