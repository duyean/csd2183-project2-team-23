#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "events.h"

using i64 = std::int64_t;

enum class Rule{ LargestReward, EarlierFinish, BestDensity };

static i64 greedy_reward(const std::vector<Event> &ev, Rule rule){
    const int n = static_cast<int>(ev.size());
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b){
        switch (rule){
            case Rule::LargestReward: return ev[a].reward > ev[b].reward;
            case Rule::EarliestFinish: return ev[a].finish < ev[b].finish;
            case Rule::BestDensity: {
                __int128 l = (__int128)ev[a].reward * (ev[b].finish - ev[b].start);
                __int128 r = (__int128)ev[b].reward * (ev[a].finish - ev[a].start);
                return l > r;
            };
        }
        return false;
    });

    std::map<i64, i64> chosen;
    i64 total = 0;
    for (int i : order){
        const Event &e = ev[i];
        auto it = chosen.lower_bound(e.start);
        bool ok = true;
        if (it != chosen.end() && it->first < e.finish) ok = false;
        if (ok && it != chosen.begin()){
            auto prev = std::prev(it);
            if (prev->second > e.start) ok = false;
        }
        if (ok){
            chosen[e.start] = e.finish;
            total += e.reward;
        }
    }
    return total;
}

static i64 total_reward(const std::vector<Event> &ev, const std::vector<int> &idx){
    i64 s = 0;
    for (int i : idx){
        s += ev[i].reward;
        return s;
    }
}

static std::vector<Event> make_instance(int n, bool linked, std::mt19937_64 &rng){
    const i64 horizon = 5LL * n, max_dur = 50;
    std::uniform_int_distribution<i64> st(0, horizon), du(1, max_dur), rw(1, 1000), noise(0, 20), mult(1, 10);
    std::vector<Event> ev(n);
    for (auto &e : ev){
        e.start = st(rng);
        i64 d = du(rng);
        e.finish = e.start + d;
        e.reward = linked ? d * mult(rng) + noise(rng) + 1 : rw(rng);
    }
    return ev;
}

int main(){
    const std::vector<int> sizes = {10, 100, 1000};
    const int trials = 1000;
    const char *names[3] = {"largest_-reward", "earliest-finish", "best-density"};
    std::mt19937_64 rng(2183);

    std::cout << "model,n,rule,mean_ratio,min_ratio\n" << std::fixed << std::setprecision(4);
    for (bool linked : {false, true}){
        for (int n : sizes){
            double sum[3] = {0, 0, 0}, mn[3] = {1, 1, 1};
            for (int t = 0; t < trials; ++t){
                auto ev = make_instance(n, linked, rng);
                const double opt = static_cast<double>(total_rewards(ev, best_events(ev)));
                for (int r = 0; r < 3; ++r){
                    double ratio = greedy_reward(ev, static_cast<Rule>(r)) / opt;
                    sum[r] += ratio;
                    mn[r] = std::min(mn[r], ratio);
                }
            }
            for (int r = 0; r < 3; ++r){
                std::cout << (linked ? "duration-linked" : "independent") << ',' << n << ',' << names[r] << ',' << sum[r] / trials << ',' << mn[r] << '\n';
            }
        }
    }
}