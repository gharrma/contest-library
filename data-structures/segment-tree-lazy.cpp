/*
 * Lazy segment tree supporting an arbitrary monoid.
 * Range updates form a monoid action over T.
 * Represented as a perfect binary tree, zero-initialized.
 * Root is 1. Parent of node i is i/2. Children of node i are 2*i and 2*i+1.
 * Leaf nodes are never lazy.
 *
 * Optimized for two types of updates:
 * - Set all elements in a range to a constant monoid element
 * - Operate over a range of elements using a constant monoid element
 *
 * s := size (number of leaves)
 * h := height
 * v := underlying array
 * lazy := unpropagated updates
 * id := monoid identity
 * op := monoid operation (not necessarily commutative)
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

template <typename Monoid>
struct seg_tree_lazy {
    using T = typename Monoid::T;
    using Act = typename Monoid::act;
    Monoid m;
    size_t s, h;
    vector<T> v;
    vector<Act> lazy;

    seg_tree_lazy(size_t n, Monoid m = Monoid()): m(m) {
        for (s = 1, h = 1; s < n; )
            s <<= 1, ++h;
        v.resize(2*s, T(m.id));
        lazy.resize(s);
    }

    template <typename Iterator>
    void set_leaves(Iterator begin, Iterator end) {
        copy(begin, end, v.begin() + s);
        for (int i = s - 1; i >= 0; --i) {
            v[i] = m.op(v[2*i], v[2*i+1]);
        }
    }

    void apply(size_t i, size_t d, const Act& a) {
        v[i] = a.apply(v[i], d);
        if (i < s) {
            lazy[i] = a.compose(lazy[i]);
        }
    }

    void push(size_t i) {
        for (size_t d = h; d > 0; --d) {
            size_t l = i >> d;
            if (!lazy[l].isNoop()) {
                apply(2*l,   d-1, lazy[l]);
                apply(2*l+1, d-1, lazy[l]);
                lazy[l] = Act();
            }
        }
    }

    void pull(size_t i) {
        for (size_t d = 1; d <= h; ++d) {
            size_t l = i >> d;
            T combined = m.op(v[2*l], v[2*l+1]);
            v[l] = lazy[l].apply(combined, d);
        }
    }

    void update(size_t i, size_t j, const Act& a) {
        i += s, j += s;
        push(i), push(j); // Needed only if updates are not commutative.
        for (size_t l = i, r = j, d = 0; l <= r; l /= 2, r /= 2, ++d) {
            if (l % 2 == 1) apply(l++, d, a);
            if (r % 2 == 0) apply(r--, d, a);
        }
        pull(i), pull(j);
    }

    T query(size_t i, size_t j) {
        i += s, j += s;
        push(i), push(j);
        T l = m.id, r = m.id;
        for (; i <= j; i /= 2, j /= 2) {
            if (i % 2 == 1) l = m.op(l, v[i++]);
            if (j % 2 == 0) r = m.op(v[j--], r);
        }
        return m.op(l, r);
    }
};

struct monoid {
    using T = int;
    static constexpr T id = 0;
    static T op(T a, T b) { return a + b; }
    static T exp(T x, size_t e) { return x * e; }
    struct act;
};

struct monoid::act {
    enum kind { kNoop, kSet, kOperate };
    kind k;
    T x;

    act(kind k = kNoop, T x = id): k(k), x(x) {}

    bool isNoop() const { return k == kNoop; }

    T apply(T n, size_t d) const {
        switch (k) {
            case kNoop:    return n;
            case kSet:     return exp(x, 1u << d);
            case kOperate: return op(n, exp(x, 1u << d));
        }
    }

    act compose(const act& other) const {
        if (other.k == kNoop || k == kSet) {
            return *this;
        } else if (k == kNoop) {
            return other;
        } else {
            assert(k == kOperate);
            return act(other.k, op(other.x, x));
        }
    }
};

int main() {
    int n = 100;
    vector<int> v(n);
    seg_tree_lazy<monoid> s(n);
    for (int t = 0; t < 1000000; ++t) {
        if (rand() % 2) {
            size_t l = rand() % n, r = rand() % n;
            int val = rand() % 100;
            auto kind = rand() % 2
                ? monoid::act::kOperate
                : monoid::act::kSet;
            for (int i = l; i <= r; ++i)
                v[i] = val + (kind == monoid::act::kOperate ? v[i] : 0);
            s.update(l, r, monoid::act(kind, val));
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
