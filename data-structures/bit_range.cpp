/*
 * Binary indexed tree, supporting range updates and point queries.
 */
template <typename T>
struct bit_range {
    vector<T> v;

    bit_range(int n): v(n+1) {}

    void update(int i, T t) {
        for (++i; i < v.size(); i += i & -i) {
            v[i] += t;
        }
    }

    void update(int a, int b, T t) {
        update(a, t);
        update(b+1, -t);
    }

    T query(int i) {
        T sum = 0;
        for (++i; i > 0; i -= i & -i)
            sum += v[i];
        return sum;
    }
};
