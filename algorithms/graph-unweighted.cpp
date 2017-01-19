/*
 * A graph data structure and various algorithms.
 * Supports directed, unweighted, and duplicate edges.
 * n := number of nodes
 * x := current node
 * c := child (neighbor) of current node
 * f := function taking in the current node and a child node. Returns
 *      whether the child node should be explored
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

    // f takes in the current node, a child node, and returns
    // whether the child node should be added to the stack.
    void dfs(int s, function<bool(int,int)> f) {
        stack<int> q;
        if (f(s, s))
            q.push(s);
        while (!q.empty()) {
            int x = q.top();
            q.pop();
            for (auto c : adj[x])
                if (f(x, c))
                    q.push(c);
        }
    }

    int cc() {
        int count = 0;
        vector<bool> seen(n);
        auto mark = [&] (int x, int c) {
            return seen[c] ? false : seen[c] = true;
        };
        for (int i = 0; i < n; ++i)
            if (!seen[i])
                dfs(i, mark), ++count;
        return count;
    }

    pair<bool, vector<bool>>
    bipartite() {
        bool ok = true;
        vector<bool> color(n);
        vector<bool> seen(n);
        auto mark = [&] (int x, int c) {
            if (seen[c]) {
                ok &= x == c || color[c] != color[x];
                return false;
            } else {
                seen[c] = true;
                color[c] = !color[x];
                return ok;
            }
        };
        for (int i = 0; i < n; ++i)
            dfs(i, mark);
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
