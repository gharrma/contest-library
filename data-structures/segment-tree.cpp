/*
 * Segment tree with arbitrary value type and operation.
 * Represented as a perfect binary tree, identity-initialized.
 * Root is 1. Parent of node i is i/2. Children of node i are 2*i and 2*i+1.
 * s := size (number of leaves)
 * v := underlying array
 * f := group operation (not necessarily commutative)
 * id := group identity
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
using namespace std;

template <typename T, typename Op>
struct seg_tree {
    size_t s;
    vector<T> v;
    Op f;

    seg_tree<T, Op>(size_t n) {
        for (s = 1; s < n; )
            s <<= 1;
        v.resize(2*s, T(Op::id));
    }

    void update(size_t i, T t) {
        i += s;
        v[i] = t;
        for (i /= 2; i > 0; i /= 2)
            v[i] = f(v[2*i], v[2*i+1]);
    }

    T query(size_t i, size_t j) {
        i += s, j += s;
        T l = Op::id, r = Op::id;
        for (; i <= j; i /= 2, j /= 2) {
            if (i % 2 == 1) l = f(l, v[i++]);
            if (j % 2 == 0) r = f(v[j--], r);
        }
        return f(l, r);
    }
};

struct op {
    static constexpr int id = 0;
    int operator()(int a, int b) { return max(a, b); }
};

int main() {
    int n = 100;
    vector<int> v(n);
    seg_tree<int, op> s(n);
    for (int t = 0; t < 1000000; ++t) {
        if (rand() % 2) {
            int i = rand() % n, val = rand() % 100;
            v[i] = val;
            s.update(i, val);
        } else {
            size_t l = rand() % n, r = rand() % n;
            if (r < l) swap(l, r);
            int max_elem = *max_element(v.begin() + l, v.begin() + r + 1);
            assert(s.query(l, r) == max_elem);
        }
    }
    cout << "All tests passed" << endl;
    return 0;
}
