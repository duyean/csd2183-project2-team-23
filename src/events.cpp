// Your event selections. Every function below is a stub that compiles but
// does nothing useful yet; replace each one with your implementation.

#include "events.h"

#include <algorithm>
#include <limits>
#include <numeric>

std::vector<int> most_events(const std::vector<Event> &events) {
  const int n = static_cast<int>(events.size());
  std::vector<int> order(n);
  std::iota(order.begin(), order.end(), 0);
  std::sort(order.begin(), order.end(), [&](int a, int b){
    return events[a].finish < events[b].finish;
  });

  std::vector<int> chosen;
  std::int64_t last = std::numeric_limits<std::int64_t>::min();
  for (int i : order){
    if (events[i].start >= last){
      chosen.push_back(i);
      last = events[i].finish;
    }
  }
  std::sort(chosen.begin(), chosen.end());
  return chosen;
}

std::vector<int> best_events(const std::vector<Event> &events) {
  const int n = static_cast<int>(events.size());
  std::vector<int> order(n);
  std::iota(order.begin(), order.end(), 0);
  std::sort(order.begin(), order.end(), [&](int a, int b){
    return events[a].finish < events[b].finish;
  });

  std::vector<std::int64_t> fin(n);
  for (int i = 0; i < n; ++i){
    fin[i] = events[order[i]].finish;
  }
  std::vector<std::int64_t> f(n + 1, 0);
  std::vector<int> p(n + 1, 0);
  std::vector<char> take(n + 1, 0);
  for (int i = 1; i <= n; ++i){
    const Event &e = events[order[i - 1]];
    p[i] = static_cast<int>(std::upper_bound(fin.begin(), fin.end(), e.start) - fin.begin());
    const std::int64_t with = e.reward + f[p[i]];
    if (with > f[i - 1]){
      f[i]= with;
      take[i] = 1;
    }
    else{
      f[i] = f[i - 1];
    }
  }

  std::vector<int> chosen;
  for (int i = n; i > 0;){
    if (take[i]){
      chosen.push_back(order[i - 1]);
      i = p[i];
    }
    else{
      --i;
    }
  }
  std::sort(chosen.begin(), chosen.end());
  return chosen;
}
