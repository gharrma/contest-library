/*
 * A graph data structure and various algorithms.
 * Supports directed, unweighted, and duplicate edges.
 * n := number of nodes
 * x := current node
 * c := child (neighbor) of current node
 *
 * Dfs: calls `f` on all nodes in the connected component of `s`, passing the
 *      current node and each of its neighbors (even if already seen)
 * Cc: returns the number of connected components in the graph
 * Bipartite: returns whether the graph is bipartite; 2-colors graph if yes
 */
#include <iostream>
#include <vector>
#include <stack>
#include <cassert>
using namespace std;

struct graph {
    int n;
    vector<vector<int>> adj;

    graph(int n): n(n), adj(n) {}

    void arc (int i, int j) { adj[i].emplace_back(j); }
    void edge(int i, int j) { arc(i, j), arc(j, i); }

    void dfs(int s, vector<bool>& seen,
             function<void(int,int)> f = [] (int x, int c) {}) {
        seen[s] = true;
        stack<int> q;
        q.push(s);
        while (!q.empty()) {
            int x = q.top();
            q.pop();
            for (auto c : adj[x]) {
                f(x, c);
                if (!seen[c]) {
                    seen[c] = true;
                    q.push(c);
                }
            }
        }
    }

    int cc() {
        vector<bool> seen(n);
        int count = 0;
        for (int i = 0; i < n; ++i)
            if (!seen[i])
                dfs(i, seen), ++count;
        return count;
    }

    bool bipartite(vector<int>& color) {
        bool ok = true;
        fill(color.begin(), color.end(), -1);
        vector<bool> seen(n);
        auto mark = [&] (int x, int c) {
            if (seen[c]) ok &= color[c] != color[x];
            else         color[c] = !color[x];
        };
        for (int i = 0; i < n; ++i)
            if (!seen[i])
                color[i] = 0, dfs(i, seen, mark);
        return ok;
    }

    bool bipartite() {
        vector<int> color(n);
        return bipartite(color);
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
    assert(g.bipartite());
    g.edge(3, 4);
    assert(g.bipartite());
    g.edge(3, 5);
    assert(!g.bipartite());
    cout << "All tests passed" << endl;
    return 0;
}
