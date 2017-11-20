/*
 * Segment tree supporting an arbitrary monoid.
 */
template <typename Monoid>
struct seg_tree {
    using T = typename Monoid::T;
    Monoid m;
    int s;
    vector<T> v;

    seg_tree(int n, Monoid m = Monoid()): m(m) {
        for (s = 1; s < n; )
            s <<= 1;
        v.resize(2*s, T(m.id));
    }

    template <typename It>
    void set_leaves(It begin, It end) {
        copy(begin, end, v.begin() + s);
        for (int i = s - 1; i >= 0; --i) {
            v[i] = m.op(v[2*i], v[2*i+1]);
        }
    }

    void update(int i, T t) {
        i += s;
        v[i] = t;
        for (i /= 2; i > 0; i /= 2) {
            v[i] = m.op(v[2*i], v[2*i+1]);
        }
    }

    T query(int i, int j) {
        i += s, j += s;
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
    static constexpr T id = numeric_limits<T>::min();
    static T op(T a, T b) { return max(a, b); }
};
