/*
 * A graph data structure and various algorithms.
 *
 * n := number of nodes
 * x := current node
 * c := child (neighbor) of current node
 * black := function to apply to elements in traversal order
 * gray := function to apply to elements at the frontier, returning true
 *         if an element should be explored.
 *
 * Cc: returns the number of connected components in the graph
 * Bipartite: returns whether the graph is bipartite; 2-colors graph if yes
 */
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cassert>
using namespace std;

struct graph {
    int n;
    vector<vector<int>> adj;

    graph(int n): n(n), adj(n) {}

    vector<int>& operator[](size_t i) { return adj[i]; }

    void arc (int i, int j) { adj[i].emplace_back(j); }
    void edge(int i, int j) { arc(i, j), arc(j, i); }

    template <typename Black, typename Gray>
    void dfs(int s, Black black, Gray gray) const {
        stack<pair<int,int>> q;
        q.emplace(-1, s);
        while (!q.empty()) {
            int x, c;
            tie(x, c) = q.top();
            q.pop();
            if (x != -1)
                black(x, c);
            for (auto cc : adj[c])
                if (gray(c, cc))
                    q.emplace(c, cc);
        }
    }

    template <typename Black>
    void dfsBlack(int s, Black black) const {
        vector<int> seen(n);
        seen[s] = true;
        dfs(s, black, [&](int x, int c) {
            return seen[c] ? false : seen[c] = true;
        });
    }

    template <typename Gray>
    void dfsGray(int s, Gray gray) const {
        return dfs(s, [](int x, int c) {}, gray);
    }

    int cc() const {
        int count = 0;
        vector<bool> seen(n);
        for (int i = 0; i < n; ++i) {
            if (!seen[i]) {
                ++count;
                seen[i] = true;
                dfsGray(i, [&](int x, int c) {
                    return seen[c] ? false : seen[c] = true;
                });
            }
        }
        return count;
    }

    pair<bool, vector<bool>>
    bipartite() const {
        bool ok = true;
        vector<bool> color(n);
        vector<bool> seen(n);
        for (int i = 0; i < n; ++i) {
            dfsGray(i, [&](int x, int c) {
                if (seen[c]) {
                    ok &= color[c] != color[x];
                    return false;
                } else {
                    seen[c] = true;
                    color[c] = !color[x];
                    return ok;
                }
            });
        }
        return make_pair(ok, color);
    }

    graph rooted(int s) const {
        graph g(n);
        dfsBlack(s, [&](int x, int c) { g.arc(x, c); });
        return g;
    }
};

int main() {
    graph g(7);
    g.edge(0, 1);
    assert(g[0][0] == 1 && g[1][0] == 0);
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
    cout << "All tests passed" << endl;
    return 0;
}
