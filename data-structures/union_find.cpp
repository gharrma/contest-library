/*
 * Union-find for integers 0 to n-1.
 * p := pointer
 * s := size
 * r := rank (approximate tree depth)
 * c := count (number of disjoint sets)
 */
struct union_find {
    vector<int> p, s, r;
    int c;

    union_find(int n): p(n), s(n, 1), r(n), c(n) {
        for (int i = 0; i < n; ++i) {
            p[i] = i;
        }
    }

    int rep(int i) {
        return p[i] == i ? i : p[i] = rep(p[i]);
    }

    void merge(int a, int b) {
        a = rep(a), b = rep(b);
        if (a == b)
            return;
        if (r[a] > r[b])
            swap(a, b);
        p[a] = b;
        s[b] += s[a];
        if (r[a] == r[b])
            ++r[b];
        --c;
    }
};
