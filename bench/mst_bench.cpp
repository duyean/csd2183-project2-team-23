#include <algorithm>
#include <chrono>
#include <cstdio>
#include <random>
#include <vector>
#include "graph.h"
#include "nanobench.h"
#include <fstream>
#include <string>
#include "maintenance.h"
using Clock = std::chrono::steady_clock;
static std::vector<Edge> connected(int n, size_t m, std::mt19937_64 &rng){
  std::uniform_int_distribution<long long> w(1,1000000);
  std::vector<Edge> e; e.reserve(m);
  for(int v=1;v<n;++v){std::uniform_int_distribution<int> p(0,v-1); e.push_back({p(rng),v,w(rng)});}
  std::uniform_int_distribution<int> x(0,n-1);
  while(e.size()<m){int a=x(rng),b=x(rng); if(a!=b) e.push_back({a,b,w(rng)});}
  return e;}
static std::vector<Edge> grid(int s,std::mt19937_64&rng){
  std::uniform_int_distribution<long long> w(1,1000000); std::vector<Edge> e;
  for(int r=0;r<s;++r)for(int c=0;c<s;++c){int v=r*s+c; if(c+1<s)e.push_back({v,v+1,w(rng)}); if(r+1<s)e.push_back({v,v+s,w(rng)});}
  return e;}

int main(int argc,char**argv){
  std::mt19937_64 rng(2183);
  ankerl::nanobench::Bench bench; bench.title("MST").unit("forest").epochs(7).epochIterations(1).warmup(1);
  auto run=[&](const char*f,int n,const std::vector<Edge>&e){
    long long a=kruskal(n,e).total_length,b=prim(n,e).total_length;
    if(a!=b){std::fprintf(stderr,"MISMATCH %s %d\n",f,n);std::exit(1);}
    std::string tag=std::string(f)+"|"+std::to_string(n)+"|"+std::to_string(e.size());
    bench.run("kruskal|"+tag,[&]{ankerl::nanobench::doNotOptimizeAway(kruskal(n,e).total_length);});
    bench.run("prim|"+tag,[&]{ankerl::nanobench::doNotOptimizeAway(prim(n,e).total_length);});};
  for(int k:{12,14,16,18}){int n=1<<k; run("sparse",n,connected(n,4ull*n,rng));}
  for(int k:{8,9,10,11,12}){int n=1<<k; run("dense",n,connected(n,(size_t)n*n/4,rng));}
  for(int s:{64,128,256,512}) run("grid",s*s,grid(s,rng));
  if(argc>1){std::ofstream o(argv[1]); bench.render(ankerl::nanobench::templates::csv(),o);}
}
