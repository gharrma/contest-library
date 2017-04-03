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
#include <cassert>
using namespace std;

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

int main() {
    int n = 1000;
    vector<int> v(n);
    for (int i = 0; i < n; ++i)
        v[i] = i;
    union_find u(n);
    for (int t = 0; t < 10000; ++t) {
        if (rand() % 2) {
            int a = rand() % n, b = rand() % n;
            u.merge(a, b);
            int rep = v[a];
            for (int i = 0; i < n; ++i)
                if (v[i] == rep)
                    v[i] = v[b];
        } else {
            int i = rand() % n;
            for (int j = 0; j < n; ++j)
                assert((u.rep(i) == u.rep(j)) == (v[i] == v[j]));
        }
    }
    cout << "All tests passed" << endl;
    return 0;
}
