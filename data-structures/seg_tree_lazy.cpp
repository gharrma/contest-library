/*
 * Lazy segment tree supporting an arbitrary monoid.
 */
template <typename Monoid>
struct seg_tree_lazy {
    using T = typename Monoid::T;
    using Update = typename Monoid::update;
    Monoid m;
    int s, h;
    vector<T> v;
    vector<Update> lazy;

    seg_tree_lazy(int n, Monoid m = Monoid()): m(m) {
        for (s = 1, h = 1; s < n; )
            s <<= 1, ++h;
        v.resize(2*s, T(m.id));
        lazy.resize(s);
    }

    template <typename InputIt>
    void set_leaves(InputIt begin, InputIt end) {
        copy(begin, end, v.begin() + s);
        for (int i = s - 1; i >= 0; --i) {
            v[i] = m.op(v[2*i], v[2*i+1]);
        }
    }

    void apply(int i, int d, const Update& u) {
        v[i] = u.apply(v[i], d);
        if (i < s) {
            lazy[i] = u.compose(lazy[i]);
        }
    }

    void push(int i) {
        for (int d = h; d > 0; --d) {
            int l = i >> d;
            if (lazy[l]) {
                apply(2*l,   d-1, lazy[l]);
                apply(2*l+1, d-1, lazy[l]);
                lazy[l] = Update();
            }
        }
    }

    void pull(int i) {
        for (int d = 1; d <= h; ++d) {
            int l = i >> d;
            T combined = m.op(v[2*l], v[2*l+1]);
            v[l] = lazy[l].apply(combined, d);
        }
    }

    void update(int i, int j, const Update& u) {
        i += s, j += s;
        push(i), push(j); // Needed if updates are not commutative.
        for (int l = i, r = j, d = 0; l <= r; l /= 2, r /= 2, ++d) {
            if (l % 2 == 1) apply(l++, d, u);
            if (r % 2 == 0) apply(r--, d, u);
        }
        pull(i), pull(j);
    }

    T query(int i, int j) {
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
    static T exp(T x, int e) { return x * e; }
    struct update;
};

struct monoid::update {
    enum kind { kNoop, kSet, kOperate };
    kind k;
    T x;

    update(kind k = kNoop, T x = id): k(k), x(x) {}

    explicit operator bool() const { return k != kNoop; }

    T apply(T n, int d) const {
        switch (k) {
            case kNoop:    return n;
            case kSet:     return exp(x, 1 << d);
            case kOperate: return op(n, exp(x, 1 << d));
        }
    }

    update compose(const update& other) const {
        if (other.k == kNoop || k == kSet) {
            return *this;
        } else if (k == kNoop) {
            return other;
        } else {
            assert(k == kOperate);
            return update(other.k, op(other.x, x));
        }
    }
};
