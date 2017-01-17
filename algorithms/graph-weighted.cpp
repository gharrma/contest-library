/*
 * A graph data structure and various algorithms.
 * Supports directed, weighted, and duplicate edges.
 * n := number of nodes
 * x := current node
 * c := child (neighbor) of current node
 * w := edge weight
 *
 * Dijkstra:
 * - returns distances from node s to every other
 * - negative edge weights not supported
 * - can use `prev` to recover paths
 *
 * Floyd-Warshall:
 * - returns distances between every pair of nodes
 * - negative cycles not supported
 * - can use `next` to recover paths
 */
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cassert>
using namespace std;

template <typename T>
struct graph {
    int n;
    vector<vector<pair<int,T>>> adj;
    static constexpr T infty = numeric_limits<T>::max() / 2;

    graph(int n): n(n), adj(n) {}

    void arc (int i, int j, T w) { adj[i].emplace_back(j, w); }
    void edge(int i, int j, T w) { arc(i, j, w), arc(j, i, w); }

    vector<T> dijkstra(int s) {
        vector<T> dist(n, T(infty));
        vector<int> prev(n, -1);
        dist[s] = 0;
        vector<bool> done(n);
        priority_queue<pair<T,int>,
                       vector<pair<T,int>>,
                       greater<pair<T,int>>> q;
        q.emplace(0, s);

        while (!q.empty()) {
            int x = q.top().second;
            q.pop();
            if (done[x])
                continue;
            done[x] = true;
            for (auto& p : adj[x]) {
                int c = p.first;
                T w = p.second;
                if (!done[c] && dist[c] > dist[x] + w) {
                    dist[c] = dist[x] + w;
                    prev[c] = x;
                    q.emplace(dist[c], c);
                }
            }
        }

        return dist;
    }

    vector<vector<T>> floyd_warshall() {
        vector<vector<T>> dist(n, vector<T>(n, T(infty)));
        vector<vector<int>> next(n, vector<int>(n, -1));
        for (int i = 0; i < n; ++i) {
            dist[i][i] = 0;
            for (auto& p : adj[i]) {
                dist[i][p.first] = min(dist[i][p.first], p.second);
                next[i][p.first] = p.first;
            }
        }

        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }

        return dist;
    }
};

int main() {
    graph<int> g(4);
    g.arc(0, 1, 2);
    g.arc(1, 2, 1);
    g.arc(0, 3, 1);
    g.arc(3, 2, 3);
    assert(g.dijkstra(0) == vector<int>({0, 2, 3, 1}));

    int n = 20;
    g = graph<int>(n);
    for (int i = 0; i < 1000; ++i) {
        if (rand() % 2) {
            g.arc(rand() % n, rand() % n, rand() % 1000);
        } else {
            int s = rand() % n;
            auto dij = g.dijkstra(s);
            auto fw = g.floyd_warshall();
            for (int i = 0; i < n; ++i)
                assert(dij[i] == fw[s][i]);
        }
    }

    cout << "All tests passed" << endl;
    return 0;
}
