// Your quest schedule. The function below is a stub that compiles but does
// nothing useful yet; replace it with your implementation.

#include "quests.h"
#include <queue>

std::optional<QuestSchedule> quest_schedule(
    const std::vector<std::int64_t> &duration,
    const std::vector<Prerequisite> &prerequisites) {
  const int n = static_cast<int>(duration.size());
  std::vector<std::vector<int>> next(n);
  std::vector<int> indeg(n, 0);
  for (const Prerequisite &pr : prerequisites){
    next[pr.before].push_back(pr.after);
    ++indeg[pr.after];
  }

  QuestSchedule result;
  result.finish.assign(n, 0);
  result.critical.assign(n, -1);

  //Kahn's algo: iterative, -> a 2*10^5 long chain cannot overflow the stack
  //When quest u is popped, all its prerequisites are done -> finish[u] is final
  std::queue<int> ready;
  for (int q = 0; q < n; ++q){
    if (indeg[q] == 0){
      ready.push(q);
    }
  }

  int done = 0;
  while (!ready.empty()){
    const int u = ready.front();
    ready.pop();
    ++done;
    const int c = result.critical[u];
    result.finish[u] = duration[u] + (c == -1 ? 0 : result.finish[c]);
    for (int v : next[u]){
      //compares against current crit prereq, not against 0 -> zero-length prereq still recorded
      if (result.critical[v] == -1 || result.finish[u] > result.finish [result.critical[v]]){
        result.critical[v] = u;
      }
      if (--indeg[v] == 0) ready.push(v);
    }
  }

  if (done < n){
    return std::nullopt; // some quest never became ready: cycle
  }
      return result;  
}
