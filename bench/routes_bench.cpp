#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <random>
#include <string>
#include <vector>

#include "bench_graphs.h"
#include "graph.h"
#include "nanobench.h"
#include "routes.h"

namespace {

struct Input {
  std::string family;
  int n;
  std::vector<Edge> edges;
};

void check_same(const Input &input) {
  const ShortestPaths a = dijkstra(input.n, input.edges, 0);
  const BellmanFordResult b = bellman_ford(input.n, input.edges, 0);
  if (!b.negative_cycle.empty() || a.dist != b.paths.dist) {
    std::fprintf(stderr, "mismatch on %s n = %d\n", input.family.c_str(),
                 input.n);
    std::exit(1);
  }
}

std::string label(const std::string &algorithm, const Input &input) {
  return algorithm + "|" + input.family + "|" + std::to_string(input.n) + "|" +
         std::to_string(input.edges.size());
}

}  // namespace

int main(int argc, char **argv) {
  std::mt19937_64 rng(2183);
  std::vector<Input> inputs;
  for (const int n : {1'024, 2'048, 4'096, 8'192, 16'384}) {
    inputs.push_back({"sparse", n, random_sparse_graph(n, std::size_t{4} * n, rng)});
  }
  for (const int side : {32, 45, 64, 91, 128}) {
    inputs.push_back({"grid", side * side, grid_graph(side, rng)});
  }

  ankerl::nanobench::Bench bench;
  bench.title("Single-source shortest paths")
      .unit("run")
      .epochs(11)
      .warmup(2)
      .minEpochTime(std::chrono::milliseconds(10));

  for (const Input &input : inputs) {
    check_same(input);
    bench.run(label("dijkstra", input), [&] {
      ankerl::nanobench::doNotOptimizeAway(
          dijkstra(input.n, input.edges, 0).dist);
    });
    bench.run(label("bellman_ford", input), [&] {
      ankerl::nanobench::doNotOptimizeAway(
          bellman_ford(input.n, input.edges, 0).paths.dist);
    });
  }

  if (argc > 1) {
    std::ofstream out(argv[1]);
    bench.render(ankerl::nanobench::templates::csv(), out);
  }
}
