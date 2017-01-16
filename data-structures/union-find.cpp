/*
 * Union-find for integers 0 to n-1.
 * p := pointer
 * s := size
 * r := rank (approximate tree depth)
 * c := count (number of disjoint sets)
 */
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct union_find {
    vector<size_t> p, s, r;
    size_t c;

    union_find(size_t n): p(n), s(n, 1), r(n), c(n) {
        for (size_t i = 0; i < n; ++i)
            p[i] = i;
    }

    size_t rep(size_t i) {
        return p[i] == i ? i : p[i] = rep(p[i]);
    }

    void merge(size_t a, size_t b) {
        a = rep(a), b = rep(b);
        if (a == b)
            return;
        if (r[a] > r[b])
            swap(a, b);
        p[a] = b;
        s[b] += s[a];
        if (r[b] == r[a])
            r[b]++;
        --c;
    }

    size_t size(size_t i) {
        return s[rep(i)];
    }
};

int main() {
    int n = 1000;
    vector<int> v(n);
    for (int i = 0; i < n; ++i)
        v[i] = i;
    union_find u(n);
    for (int t = 0; t < 1e4; ++t) {
        if (rand() % 2) {
            int a = rand() % n, b = rand() % n;
            u.merge(a, b);
            int rep = v[a];
            for (int i = 0; i < n; ++i)
                if (v[i] == rep)
                    v[i] = v[b];
        } else {
            int i = rand() % n;
            for (int j = 0; j < n; ++j) {
                if ((u.rep(i) == u.rep(j)) != (v[i] == v[j])) {
                    cout << "Test failed" << endl;
                    return 1;
                }
            }
        }
    }
    cout << "All tests passed" << endl;
    return 0;
}
