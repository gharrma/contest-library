/*
 * Lazy segment tree supporting arbitrary range updates and queries.
 * Represented as a perfect binary tree, zero-initialized.
 * Root is 1. Parent of node i is i/2. Children of node i are 2*i and 2*i+1.
 * Leaf nodes are never lazy.
 *
 * Optimized for two types of updates:
 * - Set all elements in a range to a constant group element
 * - Operate over a range of elements using a constant group element
 *
 * s := size (number of leaves)
 * h := height
 * v := underlying array
 * lazy := unpropagated updates
 * id := group identity
 * op := group operation (not necessarily commutative)
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

template <typename Group>
struct seg_tree_lazy {
    using T = typename Group::T;
    using Update = typename Group::update;
    size_t s, h;
    vector<T> v;
    vector<Update> lazy;

    seg_tree_lazy<Group>(size_t n) {
        for (s = 1, h = 1; s < n; )
            s <<= 1, ++h;
        v.resize(2*s, T(Group::id));
        lazy.resize(s);
    }

    void apply(size_t i, size_t d, const Update& u) {
        v[i] = u.apply(v[i], d);
        if (i < s)
            lazy[i] = lazy[i].compose(u);
    }

    void push(size_t i) {
        for (size_t d = h; d > 0; --d) {
            size_t l = i >> d;
            if (!lazy[l].isNoop()) {
                apply(2*l,   d-1, lazy[l]);
                apply(2*l+1, d-1, lazy[l]);
                lazy[l] = Update();
            }
        }
    }

    void pull(size_t i) {
        for (size_t d = 1; d <= h; ++d) {
            size_t l = i >> d;
            T combined = Group::op(v[2*l], v[2*l+1]);
            v[l] = lazy[l].apply(combined, d);
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
        T l = Group::id, r = Group::id;
        for (; i <= j; i /= 2, j /= 2) {
            if (i % 2 == 1) l = Group::op(l, v[i++]);
            if (j % 2 == 0) r = Group::op(v[j--], r);
        }
        return Group::op(l, r);
    }
};

struct group {
    using T = int;
    static constexpr T id = 0;
    static T op(T a, T b) { return a + b; }
    static T exp(T x, size_t e) { return x * e; }
    struct update;
};

struct group::update {
    enum kind { kNoop, kSet, kOperate };
    kind k;
    T x;

    update(kind k = kNoop, T x = id): k(k), x(x) {}

    bool isNoop() const { return k == kNoop; }

    T apply(T n, size_t d) const {
        switch (k) {
            case kNoop:    return n;
            case kSet:     return exp(x, 1u << d);
            case kOperate: return op(n, exp(x, 1u << d));
        }
    }

    update compose(const update& other) const {
        if (k == kNoop || other.k == kSet) {
            return other;
        } else if (other.k == kNoop) {
            return *this;
        } else {
            assert(other.k == kOperate);
            return update(k, op(x, other.x));
        }
    }
};

int main() {
    int n = 100;
    vector<int> v(n);
    seg_tree_lazy<group> s(n);
    for (int t = 0; t < 1000000; ++t) {
        if (rand() % 2) {
            size_t l = rand() % n, r = rand() % n;
            int val = rand() % 100;
            auto kind = rand() % 2
                ? group::update::kOperate
                : group::update::kSet;
            for (int i = l; i <= r; ++i)
                v[i] = val + (kind == group::update::kOperate ? v[i] : 0);
            s.update(l, r, group::update(kind, val));
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
