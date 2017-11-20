/*
 * Segment tree supporting delta updates.
 */
template <typename M>
struct seg_tree_delta {
    M m;
    int s;
    vector<typename M::State> v;

    seg_tree_delta(int n, M m = M()): m(m) {
        for (s = 1; s < n; )
            s <<= 1;
        v.resize(2*s);
    }

    void update(int i, typename M::Update u) {
        for (i += s; i > 0; i /= 2) {
            m.update(v[i], u);
        }
    }

    typename M::Res query(int i, int j, typename M::Query q) {
        i += s, j += s;
        auto l = m.id, r = m.id;
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
