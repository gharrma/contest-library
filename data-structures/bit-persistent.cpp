/*
 * Persistent binary indexed tree.
 * Queries with timestamp i will compute the result
 *   as if only the first i updates have been applied.
 * Remember that i & -i gives the last bit in an integer i.
 * Tree nodes start at index 1.
 * v := underlying array
 * ts := latest timestamp
 */
#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cassert>
using namespace std;

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

int main() {
    int n = 100;
    vector<int> v(n);
    bit_persistent<int> b(n);
    vector<tuple<int,int,int>> queries;
    vector<int> sums;
    for (int t = 0; t < 1000000; ++t) {
        int r = rand() % 2;
        if (r == 0) {
            int i = rand() % n;
            int val = rand() % 100;
            v[i] += val;
            b.update(i, val);
        } else {
            int l = rand() % n;
            int r = rand() % n;
            if (r < l)
                swap(l, r);
            int sum = accumulate(v.begin() + l, v.begin() + r + 1, 0);
            assert(b.query(l, r, b.ts) == sum);
            queries.emplace_back(l, r, b.ts);
            sums.push_back(sum);
        }
    }

    // Test persistence.
    for (int i = 0, e = queries.size(); i < e; ++i) {
        int l, r, ts;
        tie(l, r, ts) = queries[i];
        assert(b.query(l, r, ts) == sums[i]);
    }

    cout << "All tests passed" << endl;
    return 0;
}
