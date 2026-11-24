// Your breadth-first and depth-first searches. Every function below is a stub
// that compiles but does nothing useful yet; replace each one with your
// implementation.

#include "map_audit.h"
#include <queue>
#include <vector>

std::vector<int> region_labels(int n, const std::vector<Edge>& bridges) {
    std::vector<std::vector<int>> adj(n);

    for (const Edge& edge : bridges) {
        adj[edge.from].push_back(edge.to);
        adj[edge.to].push_back(edge.from);
    }

    std::vector<int> labels(n, -1);

    for (int start = 0; start < n; ++start) {
        if (labels[start] != -1) {
            continue;
        }

        std::queue<int> q;
        q.push(start);
        labels[start] = start;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int neighbor : adj[current]) {
                if (labels[neighbor] == -1) {
                    labels[neighbor] = start;
                    q.push(neighbor);
                }
            }
        }
    }

    return labels;
}

int fewest_hops(int n, const std::vector<Edge>& bridges, int s, int t) {
    if (s == t) {
        return 0;
    }

    std::vector<std::vector<int>> adj(n);

    for (const Edge& edge : bridges) {
        adj[edge.from].push_back(edge.to);
        adj[edge.to].push_back(edge.from);
    }

    std::vector<int> dist(n, -1);
    std::queue<int> q;

    dist[s] = 0;
    q.push(s);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int neighbor : adj[current]) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[current] + 1;

                if (neighbor == t) {
                    return dist[neighbor];
                }

                q.push(neighbor);
            }
        }
    }

    return -1;
}

