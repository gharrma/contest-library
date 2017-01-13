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

    union_find (size_t n): p(n), s(n, 1), r(n), c(n) {
        for (size_t i = 0; i < n; ++i)
            p[i] = i;
    }

    size_t rep(size_t i) {
        return p[i] == i ? i : p[i] = rep(p[i]);
    }

    void merge(size_t a, size_t b) {
        a = rep(a); b = rep(b);
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
    union_find test(10);
    test.merge(1, 5);
    test.merge(1, 3);
    test.merge(3, 3);
    cout << test.rep(3) << endl; // 5
    cout << test.c << endl; // 8
    return 0;
}
