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
    const int null = -1;
    int n;
    vector<vector<int>> adj;

    graph(int n): n(n), adj(n) {}

    void arc (int i, int j) { adj[i].emplace_back(j); }
    void edge(int i, int j) { arc(i, j), arc(j, i); }

    template <typename Black, typename Gray>
    void dfs(int s, Black black, Gray gray) {
        stack<pair<int,int>> q;
        if (gray(null, s))
            q.emplace(null, s);
        while (!q.empty()) {
            int x, c;
            tie(x, c) = q.top();
            q.pop();
            black(x, c);
            for (auto cc : adj[c])
                if (gray(c, cc))
                    q.emplace(c, cc);
        }
    }

    template <typename Black>
    void dfsBlack(int s, Black black) {
        vector<int> seen(n);
        return dfs(s, black, [&] (int x, int c) {
            return seen[c] ? false : seen[c] = true;
        });
    }

    template <typename Gray>
    void dfsGray(int s, Gray gray) {
        return dfs(s, [] (int x, int c) {}, gray);
    }

    int cc() {
        int count = 0;
        vector<bool> seen(n);
        auto mark = [&] (int x, int c) {
            return seen[c] ? false : seen[c] = true;
        };
        for (int i = 0; i < n; ++i)
            if (!seen[i])
                dfsGray(i, mark), ++count;
        return count;
    }

    pair<bool, vector<bool>>
    bipartite() {
        bool ok = true;
        vector<bool> color(n);
        vector<bool> seen(n);
        auto mark = [&] (int x, int c) {
            if (x == null) {
                return seen[c] ? false : seen[c] = true;
            } else if (seen[c]) {
                ok &= color[c] != color[x];
                return false;
            } else {
                seen[c] = true;
                color[c] = !color[x];
                return ok;
            }
        };
        for (int i = 0; i < n; ++i)
            dfsGray(i, mark);
        return make_pair(ok, color);
    }
};

int main() {
    graph g(7);
    g.edge(0, 1);
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
