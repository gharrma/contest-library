/*
 * Lazy segment tree supporting arbitrary range updates and queries.
 * Represented as a perfect binary tree, zero-initialized.
 * Root is 1. Parent of node i is i/2. Children of node i are 2*i and 2*i+1.
 * Leaf nodes are never lazy.
 *
 * s := size (number of leaves)
 * h := height
 * v := underlying array
 * lazy := unpropagated updates
 * dirty := whether a node has unpropagated updates
 * f := group operation (not necessarily commutative)
 * id := group identity
 * d := distance from the leaves
 * u := an update to a value t at distance d, supporting composition
 * push := push laziness down along a path from the root to leaf node i
 * pull := repair consistency along a path from leaf node i to the root
 */
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <cassert>
using namespace std;

template <typename T, typename Op, typename Update>
struct seg_tree_lazy {
    size_t s, h;
    vector<T> v;
    vector<Update> lazy;
    vector<bool> dirty;
    Op f;

    seg_tree_lazy<T, Op, Update>(size_t n) {
        for (s = 1, h = 1; s < n; )
            s <<= 1, ++h;
        v.resize(2*s, T(Op::id));
        lazy.resize(s);
        dirty.resize(s);
    }

    static T noop(T t, size_t d) { return t; }

    void apply(size_t i, size_t d, const Update& u) {
        v[i] = u(v[i], d);
        if (i < s) {
            lazy[i] = lazy[i] + u;
            dirty[i] = true;
        }
    }

    void push(size_t i) {
        for (size_t d = h; d > 0; --d) {
            size_t l = i >> d;
            if (dirty[l]) {
                apply(2*l,   d-1, lazy[l]);
                apply(2*l+1, d-1, lazy[l]);
                lazy[l] = Update();
                dirty[l] = false;
            }
        }
    }

    void pull(size_t i) {
        for (size_t d = 1; d <= h; ++d) {
            size_t l = i >> d;
            v[l] = lazy[l](f(v[2*l], v[2*l+1]), d);
        }
    }

    void update(size_t i, size_t j, const Update& u) {
        i += s, j += s;
        push(i), push(j); // Needed only if updates are not commutative.
        for (size_t l = i, r = j, d = 0; l <= r; l /= 2, r /= 2, ++d) {
            if (l % 2 == 1) apply(l++, d, u);
            if (r % 2 == 0) apply(r--, d, u);
        }
        pull(i), pull(j);
    }

    T query(size_t i, size_t j) {
        i += s, j += s;
        push(i), push(j);
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
    int operator()(int a, int b) { return a + b; }
};

struct update {
    enum kind { kNoop, kIncrease, kSet };
    kind k;
    int x;

    update(kind k = kNoop, int x = 0): k(k), x(x) {}

    int operator()(int n, size_t d) const {
        switch (k) {
            case kNoop:     return n;
            case kIncrease: return n + x * (1 << d);
            case kSet:      return x * (1 << d);
        }
    }

    update operator+(const update& other) const {
        if (k == kNoop || other.k == kSet) {
            return other;
        } else if (other.k == kNoop) {
            return *this;
        } else {
            assert(other.k == kIncrease);
            return update(k, x + other.x);
        }
    }
};

int main() {
    int n = 100;
    vector<int> v(n);
    seg_tree_lazy<int, op, update> s(n);
    for (int t = 0; t < 1000000; ++t) {
        if (rand() % 2) {
            size_t l = rand() % n, r = rand() % n;
            int val = rand() % 100;
            auto kind = rand() % 2 ? update::kIncrease : update::kSet;
            for (int i = l; i <= r; ++i)
                v[i] = val + (kind == update::kIncrease ? v[i] : 0);
            s.update(l, r, update(kind, val));
        } else {
            size_t l = rand() % n, r = rand() % n;
            if (r < l) swap(l, r);
            int sum = accumulate(v.begin() + l, v.begin() + r + 1, 0);
            assert(s.query(l, r) == sum);
        }
    }
    cout << "All tests passed" << endl;
    return 0;
}
