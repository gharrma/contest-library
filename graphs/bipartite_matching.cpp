/*
 * Bipartite matching using Kuhn's algorithm.
 * Runs in O(VE), but often faster.
 * Nodes on both sides are 0-indexed.
 * a := number of left nodes
 * b := number of right nodes
 */
struct graph {
    int a, b;
    vector<vector<int>> adj;
    vector<int> visit, match;

    graph(int a, int b)
        : a(a), b(b), adj(a+b), visit(a+b, -1), match(a+b, -1) {}

    void edge(int i, int j) {
        adj[i].push_back(a+j);
        adj[a+j].push_back(i);
    }

    bool augment(int n, int run) {
        if (visit[n] == run)
            return false;
        visit[n] = run;
        for (int c : adj[n]) {
            if (match[c] == -1 || augment(match[c], run)) {
                match[n] = c, match[c] = n;
                return true;
            }
        }
        return false;
    }

    int matching() {
        int ret = 0;
        for (int i = 0; i < a; ++i)
            ret += augment(i, i);
        return ret;
    }
};
