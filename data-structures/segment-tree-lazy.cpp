/*
 * Lazy segment tree supporting range updates.
 * Represented as a perfect binary tree, zero-initialized.
 * Root is 1. Parent of node i is i/2. Children of node i are 2*i and 2*i+1.
 * Leaf nodes are never lazy.
 * s := size (number of leaves)
 * h := height
 * v := underlying array
 * lazy := unpropegated updates
 * d := distance from the leaves
 */
#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
using namespace std;

template <typename T>
struct seg_tree_lazy {
    size_t s, h;
    vector<T> v, lazy;

    seg_tree_lazy<T>(size_t n) {
        s = 1, h = 1;
        while (s < n)
            s <<= 1, ++h;
        v.resize(2*s);
        lazy.resize(s);
    }

    void apply(size_t i, size_t d, T t) {
        v[i] += t * (1 << d);
        if (i < s) lazy[i] += t;
    }

    // Push laziness down along a path from the root to leaf node i.
    void push(size_t i) {
        for (size_t d = h; d > 0; --d) {
            size_t l = i >> d;
            if (lazy[l]) {
                apply(2*l,   d-1, lazy[l]);
                apply(2*l+1, d-1, lazy[l]);
                lazy[l] = 0;
            }
        }
    }

    // Repair consistency along a path from leaf node i to the root.
    void rebuild(size_t i) {
        for (size_t d = 1; d <= h; ++d) {
            size_t l = i >> d;
            v[l] = v[2*l] + v[2*l+1] + lazy[l] * (1 << d);
        }
    }

    void increase(size_t i, size_t j, T t) {
        i += s, j += s;
        for (size_t l = i, r = j, d = 0; l <= r; l /= 2, r /= 2, ++d) {
            if (l % 2 == 1) apply(l++, d, t);
            if (r % 2 == 0) apply(r--, d, t);
        }
        rebuild(i), rebuild(j);
    }

    T query(size_t i, size_t j) {
        i += s, j += s;
        push(i), push(j);
        T sum = 0;
        for (; i <= j; i /= 2, j /= 2) {
            if (i % 2 == 1) sum += v[i++];
            if (j % 2 == 0) sum += v[j--];
        }
        return sum;
    }
};

int main() {
    int n = 100;
    vector<int> v(n);
    seg_tree_lazy<int> s(n);
    for (int t = 0; t < 1e6; ++t) {
        if (rand() % 2) {
            size_t l = rand() % n, r = rand() % n;
            int val = rand() % 100;
            for (int i = l; i <= r; ++i)
                v[i] += val;
            s.increase(l, r, val);
        } else {
            size_t l = rand() % n, r = rand() % n;
            if (r < l)
                swap(l, r);
            int sum = accumulate(v.begin() + l, v.begin() + r + 1, 0);
            if (s.query(l, r) != sum) {
                cout << "Test failed" << endl;
                return 1;
            }
        }
    }
    cout << "All tests passed" << endl;
    return 0;
}
