// A benchmark with nanobench (https://nanobench.ankerl.com).
//
// Build and run it with
//
//   make bench
//   build/disjoint_set_variants times.csv links.csv
//
//It times Kruskal's algorithm with four versions of the disjoint-set forest:
//with union by rank and path compression, with only one of the two, and with
//neither. The graphs are random graphs with about 4n bridges, chains, and
//binomial graphs, each built once, outside the timed code. The times go to
//times.csv, and the average number of parent links that find follows goes to
//links.csv. Both file names are optional.
//Do not define ANKERL_NANOBENCH_IMPLEMENT; the Makefile already links the
//library into every benchmark program.

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <ostream>
#include <random>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "graph.h"
#include "maintenance.h"
#include "nanobench.h"

namespace {

//A copy of DisjointSet with union by rank and path compression switched on or off. If kCount is true, it counts the calls to find and the parent links they follow.
template <bool kByRank, bool kCompress, bool kCount>
class BenchDisjointSet {
 public:
  explicit BenchDisjointSet(int n) : parent_(n), rank_(n, 0), count_(n) 
  {
    for (int i = 0; i < n; ++i) parent_[i] = i;
  }

  int find(int x) 
  {
    if constexpr (kCount) ++finds_;
    int root = x;
    while (parent_[root] != root) 
    {
      root = parent_[root];
      if constexpr (kCount) ++links_;
    }
    if constexpr (kCompress) 
    {
      while (parent_[x] != root) 
      {
        int next = parent_[x];
        parent_[x] = root;
        x = next;
      }
    }
    return root;
  }

  bool unite(int x, int y) 
  {
    int rx = find(x);
    int ry = find(y);

    if (rx == ry) return false;


    if constexpr (kByRank) 
    {

      if (rank_[rx] < rank_[ry]) std::swap(rx, ry);

      parent_[ry] = rx;

      if (rank_[rx] == rank_[ry]) ++rank_[rx];

    } else 
    {
      parent_[rx] = ry;
    }
    --count_;

    return true;
  }

  int count() const { return count_; }

  std::int64_t finds() const { return finds_; }

  std::int64_t links() const { return links_; }

 private:
  std::vector<int> parent_;
  std::vector<int> rank_;
  int count_ = 0;
  std::int64_t finds_ = 0;
  std::int64_t links_ = 0;

};

struct Run 
{
  std::int64_t total = 0;
  std::vector<Edge> edges;
  std::int64_t finds = 0;
  std::int64_t links = 0;
};

//Kruskal's algorithm, copied from src/maintenance.cpp, with the disjoint set as a template parameter.
template <typename Sets>
Run kruskal_with(int n, const std::vector<Edge> &bridges) 
{
  Run run;
  const int m = static_cast<int>(bridges.size());

  std::vector<int> order(m);


  std::iota(order.begin(), order.end(), 0);
  std::sort(order.begin(), order.end(), [&](int a, int b) 
  {
    return std::make_pair(bridges[a].weight, a) <
           std::make_pair(bridges[b].weight, b);
  });
  Sets sets(n);

  for (int i : order) 
  {
    const Edge &e = bridges[i];

    if (sets.unite(e.from, e.to)) 
    {
      run.total += e.weight;
      run.edges.push_back(e);
      if (static_cast<int>(run.edges.size()) == n - 1) break;
    }
  }

  run.finds = sets.finds();
  run.links = sets.links();
  return run;
}

struct Input 
{
  std::string family;
  int n;
  std::vector<Edge> bridges;
};

//Collects bridges and skips self-loops and repeated bridges.
class BridgeList 
{
 public:
  void add(int u, int v, std::int64_t length) 
  {
    if (u == v) return;


    const std::uint64_t key = (static_cast<std::uint64_t>(std::min(u, v)) << 32) | static_cast<std::uint32_t>(std::max(u, v));


    if (!seen_.insert(key).second) return;

    Edge e{};
    e.from = u;
    e.to = v;
    e.weight = static_cast<decltype(e.weight)>(length);
    bridges_.push_back(e);
  }

  std::size_t size() const { return bridges_.size(); }
  std::vector<Edge> take() { return std::move(bridges_); }

 private:
  std::vector<Edge> bridges_;
  std::unordered_set<std::uint64_t> seen_;

};

//Returns a graph with n islands and 4n bridges of random length between random pairs of islands.
Input random_graph(int n, std::mt19937_64 &rng) 
{
  std::uniform_int_distribution<std::int64_t> length(0, 1'000'000'000);
  std::uniform_int_distribution<int> island(0, n - 1);
  BridgeList list;
  while (list.size() < std::size_t{4} * n) 
  {
    list.add(island(rng), island(rng), length(rng));
  }
  return {"random", n, list.take()};

}

//Returns a graph with a path through islands 0, ..., n - 2, whose bridges are the shortest, and longer bridges from island 0 to the other islands.
//Island n - 1 has no bridges, so Kruskal looks at every bridge.
Input chain_graph(int n, std::mt19937_64 &rng) 
{
  BridgeList list;

  for (int i = 0; i + 2 < n; ++i) list.add(i, i + 1, 0);
  std::vector<int> others;

  for (int k = 2; k <= n - 2; ++k) others.push_back(k);
  std::shuffle(others.begin(), others.end(), rng);
  
  for (int k : others) list.add(0, k, 1);
  return {"chain", n, list.take()};


}

//Returns a graph with 2^levels + 1 islands. The bridges (i, i + 2^l) for l = 0, 1, ... are the shortest, and union by rank turns them into binomial trees.
//They are followed by 4 * 2^levels random longer bridges. The last island has no bridges.
Input binomial_graph(int levels, std::mt19937_64 &rng) 
{
  const int core = 1 << levels;
  BridgeList list;


  for (int l = 0; l < levels; ++l) 
  {
    for (int i = 0; i + (1 << l) < core; i += 2 << l) 
    {
      list.add(i, i + (1 << l), l);
    }
  }

  std::uniform_int_distribution<int> island(0, core - 1);
  const std::size_t m = list.size() + std::size_t{4} * core;
  while (list.size() < m) list.add(island(rng), island(rng), levels);


  return {"binomial", core + 1, list.take()};
}

//Stops the program if the four variants find different spanning forests.
void check_same(const Input &input) 
{
  const Run a = kruskal_with<BenchDisjointSet<true, true, false>>(input.n, input.bridges);
  const Run b = kruskal_with<BenchDisjointSet<true, false, false>>(input.n, input.bridges);
  const Run c = kruskal_with<BenchDisjointSet<false, true, false>>(input.n, input.bridges);
  const Run d = kruskal_with<BenchDisjointSet<false, false, false>>(input.n, input.bridges);


  if (a.total != b.total || a.total != c.total || a.total != d.total || a.edges.size() != b.edges.size() || a.edges.size() != c.edges.size() || a.edges.size() != d.edges.size()) {
    std::fprintf(stderr, "mismatch on %s n = %d\n", input.family.c_str(), input.n);
    std::exit(1);
  }

}

//Writes the number of finds and parent links of one variant on one graph.
template <bool kByRank, bool kCompress>
void count_links(std::ostream &out, const std::string &variant, const Input &input) 
{
  const Run run = kruskal_with<BenchDisjointSet<kByRank, kCompress, true>>(input.n, input.bridges);


      out << variant << ',' << input.family << ',' << input.n << ','
      << input.bridges.size() << ',' << run.finds << ',' << run.links << ','
      << static_cast<double>(run.links) / static_cast<double>(run.finds)
      << '\n';

}

//Times one variant on one graph and adds the result to bench.
template <bool kByRank, bool kCompress>
void benchmark_variant(ankerl::nanobench::Bench &bench, const std::string &variant, const Input &input) 
{
  bench.run(variant + "|" + input.family + "|" + std::to_string(input.n) + "|" + std::to_string(input.bridges.size()),
            [&] {
              ankerl::nanobench::doNotOptimizeAway(kruskal_with<BenchDisjointSet<kByRank, kCompress, false>>(input.n, input.bridges).total);
            });
}

}  // namespace

int main(int argc, char **argv) 
{
  std::mt19937_64 rng(2183);
  std::vector<Input> inputs;
  for (const int k : {10, 12, 14, 16}) 
  {
    inputs.push_back(random_graph(1 << k, rng));
  }
  for (const int k : {10, 11, 12, 13}) 
  {
    inputs.push_back(chain_graph(1 << k, rng));
  }
  for (const int k : {10, 12, 14, 16}) 
  {
    inputs.push_back(binomial_graph(k, rng));
  }

  //Every measurement repeats the computation for at least 10 ms, so that small graphs give stable results, and nanobench reports the median of 11 measurements.
  ankerl::nanobench::Bench bench;
  bench.title("Disjoint-set variants in Kruskal")
      .unit("forest")
      .epochs(11)
      .warmup(1)
      .minEpochTime(std::chrono::milliseconds(10));

  std::ofstream links;
  if (argc > 2) 
  {
    links.open(argv[2]);
  }
  links << "variant,family,n,m,finds,links,avg_links\n";

  for (const Input &input : inputs) 
  {
    check_same(input);
    count_links<true, true>(links, "both", input);
    count_links<true, false>(links, "rank", input);
    count_links<false, true>(links, "compress", input);
    count_links<false, false>(links, "neither", input);
    benchmark_variant<true, true>(bench, "both", input);
    benchmark_variant<true, false>(bench, "rank", input);
    benchmark_variant<false, true>(bench, "compress", input);
    benchmark_variant<false, false>(bench, "neither", input);
  }

  if (argc > 1) 
  {
    std::ofstream out(argv[1]);
    bench.render(ankerl::nanobench::templates::csv(), out);
  }
}
