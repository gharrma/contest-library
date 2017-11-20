/*
 * Binary indexed tree.
 */
template <typename T>
struct bit {
    int n;
    vector<T> v;

    bit(int n): n(n+1), v(n+1) {}

    void update(int i, T t) {
        for (++i; i < n; i += i & -i) {
            v[i] += t;
        }
    }

    T query(int i) {
        T sum = 0;
        for (++i; i > 0; i -= i & -i)
            sum += v[i];
        return sum;
    }

    T query(int l, int r) {
        return l <= r ? query(r) - query(l-1) : 0;
    }

    int lower_bound(T x) {
        int i = 0, step = 1;
        while (step < n)
            step <<= 1;
        for (; step > 0; step >>= 1)
            if (i + step < n && v[i + step] < x)
                i += step, x -= v[i];
        return i;
    }
};
