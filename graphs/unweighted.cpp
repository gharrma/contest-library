/*
 * A graph data structure and various algorithms.
 * All traversals are non-recursive.
 * n := number of nodes
 * counter := auxiliary counter for traversal isolation
 * visit := auxiliary array for tracking visited nodes
 * next := auxiliary array for tracking next DFS child to visit
 * p := parent node
 * x := current node
 * nbr := neighbor of the current node
 * cc := connected components
 * scc := strongly connected components
 * lca := lowest common ancestor
 */
void noop(int p, int x) {}

struct graph {
    int n;
    vector<vector<int>> adj;
    mutable int counter;
    mutable vector<int> visit, next;

    graph(int n): n(n), adj(n), counter(0), visit(n), next(n) {}

    void arc (int i, int j) { adj[i].emplace_back(j); }
    void edge(int i, int j) { arc(i, j), arc(j, i); }

    template <typename Pre, typename Post>
    void dfs(int s, Pre pre, Post post) const;

    template <typename Pre, typename Post>
    void dfs(Pre pre, Post post) const;

    template <typename F>
    void bfs(int s, F f) const;

    template <typename F>
    void bfs(F f) const;

    template <typename F>
    void euler_tour(int s, F f) const;

    int cc() const;

    vector<int> scc() const;

    pair<bool, vector<bool>> bipartite() const;

    vector<int> articulation_points() const;

    graph dfs_tree(int root) const;

    vector<pair<int,int>> push_pop_order(int root) const;

    function<int(int,int)> lca(int root) const;
};

template <typename Pre, typename Post>
void graph::dfs(int s, Pre pre, Post post) const {
    ++counter;
    if (visit[s] == counter)
        return;
    stack<pair<int,int>> q;
    q.emplace(-1, s);
    pre(-1, s);
    visit[s] = counter;
    next[s] = 0;
    while (!q.empty()) {
        int p, x;
        tie(p, x) = q.top();
        if (next[x] == adj[x].size()) {
            q.pop();
            post(p, x);
        } else while (next[x] < adj[x].size()) {
            int nbr = adj[x][next[x]++];
            if (visit[nbr] != counter) {
                q.emplace(x, nbr);
                pre(x, nbr);
                visit[nbr] = counter;
                next[nbr] = 0;
                break;
            }
        }
    }
}

template <typename Pre, typename Post>
void graph::dfs(Pre pre, Post post) const {
    for (int i = 0; i < n; ++i) {
        if (i) --counter;
        dfs(i, pre, post);
    }
}

template <typename F>
void graph::bfs(int s, F f) const {
    ++counter;
    if (visit[s] == counter)
        return;
    queue<pair<int,int>> q;
    q.emplace(-1, s);
    visit[s] = counter;
    while (!q.empty()) {
        int p, x;
        tie(p, x) = q.front();
        q.pop();
        f(p, x);
        for (auto nbr : adj[x]) {
            if (visit[nbr] != counter) {
                q.emplace(x, nbr);
                visit[nbr] = counter;
            }
        }
    }
}

template <typename F>
void graph::bfs(F f) const {
    for (int i = 0; i < n; ++i) {
        if (i) --counter;
        bfs(i, f);
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
    bfs([&](int p, int x) { if (p == -1) ++count; });
    return count;
}

graph graph::dfs_tree(int root) const {
    graph g(n);
    dfs(root, noop, [&](int p, int x) { if (p != -1) g.arc(p, x); });
    return g;
}

pair<bool, vector<bool>> graph::bipartite() const {
    vector<bool> color(n);
    bfs([&](int p, int x) {
        color[x] = p == -1 ? false : !color[p];
    });
    bool ok = true;
    for (int i = 0; i < n; ++i)
        for (auto nbr : adj[i])
            ok &= color[i] != color[nbr];
    return make_pair(ok, color);
}

vector<int> graph::articulation_points() const {
    // To find bridges, must also check for cliques of size 2.
    vector<int> parent(n), dist(n), low(n);
    vector<int> points;

    auto pre = [&](int p, int x) {
        parent[x] = p;
        dist[x] = p == -1 ? 1 : dist[p] + 1;
    };

    auto post = [&](int p, int x) {
        if (p == -1) {
            // Root vertex is an articulation point if it has
            // more than one DFS child.
            int deg = 0;
            for (auto nbr : adj[x])
                if (parent[nbr] == x)
                    ++deg;
            if (deg > 1) {
                points.push_back(x);
            }
        } else {
            // Non-root vertex x is an articulation point if some
            // DFS child nbr exists with low[nbr] >= dist[x].
            low[x] = dist[x];
            for (auto nbr : adj[x]) {
                if (nbr != p)
                    low[x] = min(low[x], dist[nbr]);
                if (parent[nbr] == x) {
                    low[x] = min(low[x], low[nbr]);
                    if (low[nbr] >= dist[x]) {
                        points.push_back(x);
                    }
                }
            }
        }
    };

    dfs(pre, post);
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());
    return points;
}

vector<int> graph::scc() const {
    vector<int> q(n);
    int idx = n;
    dfs(noop, [&](int p, int x) { q[--idx] = x; });
    graph transpose(n);
    for (int i = 0; i < n; ++i)
        for (auto nbr : adj[i])
            transpose.arc(nbr, i);
    vector<int> res(n, -1);
    for (int i = 0; i < n; ++i) {
        if (i) --transpose.counter; // Keep marked nodes marked.
        transpose.bfs(q[i], [&](int p, int x) { res[x] = q[i]; });
    }
    return res;
}

vector<pair<int,int>> graph::push_pop_order(int root) const {
    vector<pair<int,int>> order(n);
    int i = 0;
    auto pre  = [&](int p, int x) { order[x].first  = i++; };
    auto post = [&](int p, int x) { order[x].second = i++; };
    dfs(root, pre, post);
    return order;
}

function<int(int,int)> graph::lca(int root) const {
    int log = 0;
    while (1 << log < n)
        ++log;

    // dp[k][i] := (2^k)th parent of node i, or -1 if none.
    vector<vector<int>> dp(log + 1, vector<int>(n, -1));
    bfs(root, [&](int p, int x) { dp[0][x] = p; });
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
        return a == b ? a : dp[0][a];
    };
}
