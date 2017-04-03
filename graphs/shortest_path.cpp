/*
 * A graph data structure with shortest path algorithms.
 * Supports directed, weighted, and duplicate edges.
 * n := number of nodes
 * x := current node
 * c := child (neighbor) of current node
 * w := edge weight
 *
 * Dijkstra: ~ O(E log V)
 * - returns distances from a source node s to every other
 * - negative edges not supported
 *
 * Bellman-Ford: ~ O(VE)
 * - returns distances from a source node s to every other
 * - supports negative edges
 *
 * Floyd-Warshall: O(V^3)
 * - returns distances between every pair of nodes
 * - supports negative edges
 *
 * Johnson's Algorithm: ~ O(V^2 log V + VE)
 * - returns distances between every pair of nodes
 * - better than Floyd-Warshall for sparse graphs with negative edges
 *
 * Mst:
 * - returns the weight of the minimum spanning tree
 */
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <tuple>
#include <cassert>
using namespace std;

template <typename T>
struct graph {
    int n;
    vector<vector<pair<int,T>>> adj;
    static constexpr T infty = numeric_limits<T>::max();

    graph(int n): n(n), adj(n) {}

    void arc (int i, int j, T w) { adj[i].emplace_back(j, w); }
    void edge(int i, int j, T w) { arc(i, j, w), arc(j, i, w); }

    pair<vector<T>, vector<int>>
    dijkstra(int s) const;

    pair<vector<T>, vector<int>>
    bellman_ford(int s, bool* negative_cycle) const;

    pair<vector<vector<T>>, vector<vector<int>>>
    floyd_warshall() const;

    pair<vector<vector<T>>, vector<vector<int>>>
    johnsons_algorithm(bool* negative_cycle) const;

    T mst() const;
};

template <typename T>
pair<vector<T>, vector<int>>
graph<T>::dijkstra(int s) const {
    vector<T> dist(n, T(infty));
    vector<int> prev(n, -1);
    vector<bool> done(n);
    priority_queue<pair<T,int>,
                   vector<pair<T,int>>,
                   greater<pair<T,int>>> q;
    q.emplace(0, s);
    dist[s] = 0;

    while (!q.empty()) {
        int x = q.top().second;
        q.pop();
        if (done[x])
            continue;
        done[x] = true;
        for (auto& p : adj[x]) {
            int c; T w;
            tie(c, w) = p;
            if (!done[c] && dist[c] > dist[x] + w) {
                dist[c] = dist[x] + w;
                prev[c] = x;
                q.emplace(dist[c], c);
            }
        }
    }

    return make_pair(dist, prev);
}

template <typename T>
pair<vector<T>, vector<int>>
graph<T>::bellman_ford(int s, bool* negative_cycle) const {
    *negative_cycle = false;
    vector<T> dist(n, T(infty));
    vector<int> prev(n, -1);
    dist[s] = 0;

    for (int i = 0; i < n; ++i)
        for (int x = 0; x < n; ++x)
            for (auto& p : adj[x])
                if (dist[x] < infty)
                    if (dist[p.first] > dist[x] + p.second) {
                        dist[p.first] = dist[x] + p.second;
                        prev[p.first] = x;
                        if (i == n - 1) {
                            *negative_cycle = true;
                        }
                    }

    return make_pair(dist, prev);
}

template <typename T>
pair<vector<vector<T>>, vector<vector<int>>>
graph<T>::floyd_warshall() const {
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
                if (dist[i][k] < infty && dist[k][j] < infty)
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }

    return make_pair(dist, next);
}

template <typename T>
pair<vector<vector<T>>, vector<vector<int>>>
graph<T>::johnsons_algorithm(bool* negative_cycle) const {
    *negative_cycle = false;
    vector<vector<T>> dist(n);
    vector<vector<int>> prev(n);

    graph<T> alt = *this;
    alt.n += 1;
    alt.adj.resize(alt.n);
    for (int i = 0; i < n; ++i)
        alt.arc(n, i, 0);
    auto q_dist = alt.bellman_ford(n, negative_cycle).first;
    if (*negative_cycle)
        return make_pair(dist, prev);

    alt.n -= 1;
    alt.adj.resize(alt.n);
    for (int i = 0; i < n; ++i)
        for (auto& p : alt.adj[i])
            p.second += q_dist[i] - q_dist[p.first];

    for (int s = 0; s < n; ++s)
        tie(dist[s], prev[s]) = alt.dijkstra(s);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (dist[i][j] < infty)
                dist[i][j] -= q_dist[i] - q_dist[j];

    return make_pair(dist, prev);
}

template <typename T>
T graph<T>::mst() const {
    vector<bool> done(n);
    priority_queue<pair<T,int>,
                   vector<pair<T,int>>,
                   greater<pair<T,int>>> q;
    q.emplace(0, 0);
    T ret = 0;

    while (!q.empty()) {
        T w; int c;
        tie(w, c) = q.top();
        q.pop();
        if (!done[c]) {
            done[c] = true;
            ret += w;
            for (auto& p : adj[c]) {
                q.emplace(p.second, p.first);
            }
        }
    }

    return ret;
}

int main() {
    graph<int> g(4);
    g.arc(0, 1, 2);
    g.arc(1, 2, 1);
    g.arc(0, 3, 1);
    g.arc(3, 2, 3);
    assert(g.dijkstra(0).first == vector<int>({0, 2, 3, 1}));
    assert(g.mst() == 4);

    g = graph<int>(3);
    g.edge(0, 1, 3);
    g.edge(1, 2, -2);
    g.edge(2, 0, -2);
    bool neg_cycle;
    g.bellman_ford(0, &neg_cycle);
    assert(neg_cycle);
    g.johnsons_algorithm(&neg_cycle);
    assert(neg_cycle);
    assert(g.mst() == -4);

    int n = 30;
    g = graph<int>(n);
    for (bool neg_allowed : {false, true}) {
        for (int i = 0; i < 1000; ++i) {
            if (rand() % 2) {
                int weight = rand() % 1000 - (neg_allowed ? 100 : 0);
                g.arc(rand() % n, rand() % n, weight);
            } else {
                int s = rand() % n;
                auto bf = g.bellman_ford(s, &neg_cycle).first;
                auto fw = g.floyd_warshall().first;
                auto ja = g.johnsons_algorithm(&neg_cycle).first;
                if (neg_allowed) {
                    if (neg_cycle) {
                        g = graph<int>(n);
                        continue;
                    }
                    for (int i = 0; i < n; ++i)
                        for (auto dist : {fw[s][i], ja[s][i]})
                            assert(dist == bf[i]);
                } else {
                    auto dij = g.dijkstra(s).first;
                    for (int i = 0; i < n; ++i)
                        for (auto dist : {bf[i], fw[s][i], ja[s][i]})
                            assert(dist == dij[i]);
                }
            }
        }
    }

    cout << "All tests passed" << endl;
    return 0;
}
