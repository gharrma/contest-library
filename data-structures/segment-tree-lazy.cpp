/*
 * Lazy segment tree supporting range updates.
 * Represented as a perfect binary tree, zero-initialized.
 * Root is 1. Parent of node i is i/2. Children of node i are 2*i and 2*i + 1.
 * Leaf nodes are never lazy.
 * s := size (number of leaves)
 * h := height
 * v := underlying array
 * lazy := unpropegated updates
 */
#include <iostream>
#include <algorithm>
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

    void apply(size_t i, T t) {
        (i < s ? lazy : v)[i] += t;
    }

    T total(size_t i) {
        return v[i] + (i < s ? lazy[i] : 0);
    }

    // Push laziness down along a path from the root to node i.
    void push(size_t i) {
        for (size_t d = h; d > 0; d--) {
            size_t l = i >> d;
            if (lazy[l]) {
                v[l] += (1 << d) * lazy[l];
                apply(2*l, lazy[l]);
                apply(2*l + 1, lazy[l]);
                lazy[l] = 0;
            }
        }
    }

    // Rebuild integrity along a path from node i to the root.
    void rebuild(size_t i) {
        for (i /= 2; i > 0; i /= 2)
            v[i] = total(2*i) + total(2*i + 1);
    }

    void increase(size_t i, size_t j, T t) {
        i += s, j += s;
        push(i), push(j);
        for (size_t l = i, r = j; l <= r; l /= 2, r /= 2) {
            if (l % 2 == 1) apply(l++, t);
            if (r % 2 == 0) apply(r--, t);
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
    seg_tree_lazy<int> test(5);
    test.increase(0, 0, 1);
    test.increase(1, 1, 2);
    test.increase(2, 2, 3);
    test.increase(3, 3, 4);
    test.increase(4, 4, 5);
    test.increase(4, 4, 5);
    cout << test.query(0, 0) << endl; // 1
    cout << test.query(0, 3) << endl; // 10
    cout << test.query(0, 4) << endl; // 20
    cout << test.query(1, 3) << endl; // 9
    cout << test.query(4, 4) << endl; // 10
    return 0;
}
