/*
 * Persistent binary indexed tree.
 * Queries with timestamp i will compute the result
 *   as if only the first i updates have been applied.
 */
template <typename T>
struct bit_persistent {
    using History = vector<pair<int,T>>;
    vector<History> v;
    int ts;

    bit_persistent(int n)
        : v(n+1, History({make_pair(0, T(0))}))
        , ts(0) {}

    void update(int i, T t) {
        for (++ts, ++i; i < v.size(); i += i & -i) {
            v[i].emplace_back(ts, v[i].back().second + t);
        }
    }

    T query(int i, int time) {
        T sum = 0;
        auto key = make_pair(time, numeric_limits<T>::max());
        for (++i; i > 0; i -= i & -i)
            sum += prev(upper_bound(v[i].begin(), v[i].end(), key))->second;
        return sum;
    }

    T query(int l, int r, int time) {
        return l <= r ? query(r, time) - query(l-1, time) : 0;
    }
};
