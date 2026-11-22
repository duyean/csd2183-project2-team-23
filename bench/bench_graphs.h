#ifndef BENCH_GRAPHS_H
#define BENCH_GRAPHS_H

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

#include "graph.h"

inline std::vector<Edge> random_sparse_graph(int n, std::size_t m,
                                             std::mt19937_64 &rng) {
  std::uniform_int_distribution<std::int64_t> weight(1, 1'000'000);
  std::vector<Edge> edges;
  edges.reserve(m);
  for (int v = 1; v < n; ++v) {
    std::uniform_int_distribution<int> earlier(0, v - 1);
    edges.push_back({earlier(rng), v, weight(rng)});
  }
  std::uniform_int_distribution<int> vertex(0, n - 1);
  while (edges.size() < m) {
    edges.push_back({vertex(rng), vertex(rng), weight(rng)});
  }
  return edges;
}

inline std::vector<Edge> grid_graph(int side, std::mt19937_64 &rng) {
  std::uniform_int_distribution<std::int64_t> weight(1, 1'000'000);
  std::vector<Edge> edges;
  edges.reserve(static_cast<std::size_t>(4) * side * side);
  for (int r = 0; r < side; ++r) {
    for (int c = 0; c < side; ++c) {
      const int v = r * side + c;
      if (c + 1 < side) {
        edges.push_back({v, v + 1, weight(rng)});
        edges.push_back({v + 1, v, weight(rng)});
      }
      if (r + 1 < side) {
        edges.push_back({v, v + side, weight(rng)});
        edges.push_back({v + side, v, weight(rng)});
      }
    }
  }
  return edges;
}

#endif
