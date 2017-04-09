/*
 * Segment tree supporting delta updates.
 * Represented as a perfect binary tree, identity-initialized.
 * Root is 1. Parent of node i is i/2. Children of node i are 2*i and 2*i+1.
 * s := size (number of leaves)
 * v := underlying array
 * id := result identity
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cassert>
using namespace std;

template <typename M>
struct seg_tree_delta {
    using State = typename M::State;
    using Update = typename M::Update;
    using Query = typename M::Query;
    using Res = typename M::Res;
    M m;
    int s;
    vector<State> v;

    seg_tree_delta(int n, M m = M()): m(m) {
        for (s = 1; s < n; )
            s <<= 1;
        v.resize(2*s);
    }

    void update(int i, Update u) {
        for (i += s; i > 0; i /= 2) {
            m.update(v[i], u);
        }
    }

    Res query(int i, int j, Query q) {
        i += s, j += s;
        Res l = m.id, r = m.id;
        for (; i <= j; i /= 2, j /= 2) {
            if (i % 2 == 1) l = m.compose(l, m.query(v[i++], q));
            if (j % 2 == 0) r = m.compose(m.query(v[j--], q), r);
        }
        return m.compose(l, r);
    }
};

struct M {
    using State = unordered_map<int,int>;
    using Update = int;
    using Query = int;
    using Res = int;
    static constexpr Res id = 0;

    static void update(State& state, Update val) {
        ++state[val];
    }

    static Res query(State& state, Query val) {
        return state[val];
    }

    static Res compose(Res a, Res b) {
        return a + b;
    }
};

int main() {
    int n = 100;
    vector<unordered_map<int,int>> v(n);
    seg_tree_delta<M> s(n);
    for (int t = 0; t < 1000000; ++t) {
        int val = rand() % 100;
        if (rand() % 2) {
            int i = rand() % n;
            ++v[i][val];
            s.update(i, val);
        } else {
            int l = rand() % n;
            int r = rand() % n;
            if (r < l)
                swap(l, r);
            int res = 0;
            for(int i = l; i <= r; ++i)
                res += v[i][val];
            assert(s.query(l, r, val) == res);
        }
    }
    cout << "All tests passed" << endl;
    return 0;
}
