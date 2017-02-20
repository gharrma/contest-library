/*
 * A graph data structure and various algorithms.
 * n := number of nodes
 * p := parent node
 * x := current node
 * cc := connected components
 */
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <cassert>
using namespace std;

struct graph {
    int n;
    vector<vector<int>> adj;

    graph(int n): n(n), adj(n) {}

    void arc (int i, int j) { adj[i].emplace_back(j); }
    void edge(int i, int j) { arc(i, j), arc(j, i); }

    template <typename Pre, typename Post>
    void dfs(int s, Pre pre, Post post) const;

    template <typename Pre>
    void dfsPre(int s, Pre pre) const { dfs(s, pre, [](int p, int x) {}); }

    template <typename Post>
    void dfsPost(int s, Post post) const { dfs(s, [](int p, int x) {}, post); }

    template <typename F>
    void bfs(int s, F f) const;

    template <typename F>
    void euler_tour(int s, F f) const;

    int cc() const;

    pair<bool, vector<bool>> bipartite() const;

    graph rooted(int root) const;

    vector<pair<int,int>> push_pop_order(int root) const;

    function<int(int,int)> lca(int root) const;
};

template <typename Pre, typename Post>
void graph::dfs(int s, Pre pre, Post post) const {
    unordered_map<int,bool> gray, black;
    stack<pair<int,int>> q;
    q.emplace(-1, s);
    while (!q.empty()) {
        int p, x;
        tie(p, x) = q.top();
        if (black[x]) {
            q.pop();
            continue;
        } else if (gray[x]) {
            black[x] = true;
            q.pop();
            post(p, x);
        } else {
            gray[x] = true;
            pre(p, x);
            for (auto c : adj[x]) {
                if (!gray[c]) {
                    q.emplace(x, c);
                }
            }
        }
    }
}

template <typename F>
void graph::bfs(int s, F f) const {
    unordered_map<int,bool> seen;
    queue<pair<int,int>> q;
    q.emplace(-1, s);
    while (!q.empty()) {
        int p, x;
        tie(p, x) = q.front();
        q.pop();
        f(p, x);
        for (auto c : adj[x]) {
            if (!seen[c]) {
                seen[c] = true;
                q.emplace(x, c);
            }
        }
    }
}

template <typename F>
void graph::euler_tour(int s, F f) const {
    auto pre  = [&](int p, int x) { if (p != -1) f(p); };
    auto post = [&](int p, int x) { f(x); };
    dfs(s, pre, post);
}

int graph::cc() const {
    int count = 0;
    vector<bool> seen(n);
    for (int i = 0; i < n; ++i) {
        if (!seen[i]) {
            ++count;
            bfs(i, [&](int p, int x) { seen[x] = true; });
        }
    }
    return count;
}

graph graph::rooted(int root) const {
    graph g(n);
    bfs(root, [&](int p, int x) { if (p != -1) g.arc(p, x); });
    return g;
}

pair<bool, vector<bool>> graph::bipartite() const {
    bool ok = true;
    vector<bool> seen(n), color(n);
    auto pre = [&](int p, int x) {
        seen[x] = true;
        color[x] = p == -1 ? false : !color[p];
    };
    for (int i = 0; i < n; ++i)
        if (!seen[i])
            bfs(i, pre);
    for (int i = 0; i < n; ++i)
        for (auto c : adj[i])
            ok &= color[i] != color[c];
    return make_pair(ok, color);
}

vector<pair<int,int>> graph::push_pop_order(int root) const {
    vector<pair<int,int>> order(n);
    int counter = 0;
    auto pre  = [&](int p, int x) { order[x].first  = counter++; };
    auto post = [&](int p, int x) { order[x].second = counter++; };
    dfs(root, pre, post);
    return order;
}

function<int(int,int)> graph::lca(int root) const {
    int log = 0;
    while (1 << log < n)
        ++log;

    // dp[k][i] := (2^k)th parent of node i, or -1 if none.
    vector<vector<int>> dp(log + 1, vector<int>(n, -1));
    bfs(root, [&](int p, int x) {
        dp[0][x] = p;
    });
    for (int k = 1; k <= log; ++k)
        for (int i = 0; i < n; ++i)
            if (dp[k-1][i] != -1)
                dp[k][i] = dp[k-1][dp[k-1][i]];

    vector<int> dist(n);
    bfs(root, [&](int p, int x) {
        dist[x] = p == -1 ? 0 : dist[p] + 1;
    });

    return [=](int a, int b) {
        if (dist[a] < dist[b])
            swap(a, b);
        for (int k = log; k >= 0; --k)
            if (dp[k][a] != -1 && dist[dp[k][a]] >= dist[b])
                a = dp[k][a];
        for (int k = log; k >= 0; --k)
            if (dp[k][a] != dp[k][b])
                a = dp[k][a], b = dp[k][b];
        return dp[0][a];
    };
}

int main() {
    graph g(7);
    g.edge(0, 1);
    assert(g.adj[0][0] == 1 && g.adj[1][0] == 0);
    g.edge(0, 3);
    g.edge(1, 2);
    g.edge(2, 3);
    g.edge(4, 5);
    assert(g.cc() == 3);
    assert(g.bipartite().first);
    g.edge(3, 4);
    assert(g.bipartite().first);
    g.edge(3, 5);
    assert(!g.bipartite().first);

    // graph::push_pop_order tested on HackerRank, "The Story of a Tree."

    cout << "All tests passed" << endl;
    return 0;
}
