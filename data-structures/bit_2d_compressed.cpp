/*
 * 2-dimensional coordinate-compressed Fenwick tree.
 * Regardless of coordinate magnitudes, preprocessing is
 *   O(n lg n) and updates/queries are O(lg n lg n).
 * Significantly faster than sparse Fenwick tree, but is offline.
 * Add all coordinates that will be updated, then call build().
 *
 * pts := vector of all coordinates that will be touched
 * xs := sorted vector of distinct x-coordinates
 * ys[i] := sorted vector of distinct y-coordinates from
 *          points with the ith lowest x-coordinate
 * v := Fenwick tree nodes
 * idx := returns the 1-based index of the last element no greater than val
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
using namespace std;
using ll = long long;

template <typename T>
struct bit_2d_compressed {
    using pt = pair<ll,ll>;
    vector<pair<ll,ll>> pts;
    vector<ll> xs;
    vector<vector<ll>> ys;
    vector<vector<T>> v;

    void add(ll x, ll y) {
        pts.emplace_back(x, y);
    }

    void build() {
        // Compress first component.
        sort(pts.begin(), pts.end(), [](const pt& a, const pt& b) {
            return a.first < b.first;
        });
        for (const auto& p : pts)
            if (xs.empty() || p.first != xs.back())
                xs.push_back(p.first);
        ys.resize(xs.size() + 1);
        v.resize(xs.size() + 1);

        // Compress second component.
        sort(pts.begin(), pts.end(), [](const pt& a, const pt& b) {
            return a.second < b.second;
        });
        for (const auto& p : pts)
            for (auto i = idx(xs, p.first); i < v.size(); i += i & -i)
                if (ys[i].empty() || p.second != ys[i].back())
                    ys[i].push_back(p.second);
        for (int i = 1; i < v.size(); ++i) {
            v[i].resize(ys[i].size() + 1);
        }
    }

    void update(ll x, ll y, T t) {
        for (int i = idx(xs, x); i < v.size(); i += i & -i) {
            for (int j = idx(ys[i], y); j < v[i].size(); j += j & -j) {
                v[i][j] += t;
            }
        }
    }

    T query(ll x, ll y) const {
        T res = 0;
        for (int i = idx(xs, x); i > 0; i -= i & -i)
            for (int j = idx(ys[i], y); j > 0; j -= j & -j)
                res += v[i][j];
        return res;
    }

    T query(ll x1, ll y1, ll x2, ll y2) const {
        if (x2 < x1 || y2 < y1)
            return 0;
        return query(x2, y2) - query(x2, y1-1)
                             - query(x1-1, y2) + query(x1-1, y1-1);
    }

    int idx(const vector<ll>& v, ll val) const {
        auto ub = upper_bound(v.begin(), v.end(), val);
        return distance(v.begin(), ub);
    }
};

int main() {
    bit_2d_compressed<int> s;
    s.add(0,1), s.add(0,15), s.add(7,7), s.add(12,13);
    s.add(123456789,987654321);
    s.add(123456789,999999999);
    s.add(999999999,123456789);
    s.build();
    assert(s.query(0,1,30,15) == 0);
    s.update(0,1,1);
    s.update(0,15,1);
    s.update(7,7,1);
    s.update(7,7,1);
    assert(s.query(0,1,30,15) == 4);
    s.update(7,7,3);
    s.update(12, 13, 1);
    s.update(123456789,987654321,1);
    s.update(123456789,999999999,2);
    s.update(999999999,123456789,1);
    assert(s.query(0,0) == 0);
    assert(s.query(0,0,0,1) == 1);
    assert(s.query(0,0,1,1) == 1);
    assert(s.query(0,0,1,15) == 2);
    assert(s.query(0,7,1,32) == 1);
    assert(s.query(0,1,30,15) == 8);
    assert(s.query(12345,0,999999998,987654321) == 1);
    assert(s.query(12345,0,999999998,1000000000) == 3);
    assert(s.query(0,0,1000000000,1000000000) == 12);

    // Test speed.
    s = bit_2d_compressed<int>();
    vector<tuple<int,int,int>> q;
    for (int t = 0; t < 50000; ++t) {
        int x = rand();
        int y = rand();
        int val = rand() % 100;
        q.emplace_back(x, y, val);
        s.add(x, y);
    }
    for (int t = 0; t < 50000; ++t) {
        q.emplace_back(123456, t, 1);
        s.add(123456, t);
    }
    s.build();
    for (auto& p : q) {
        int x, y, val;
        tie(x, y, val) = p;
        s.update(x, y, val);
        int x1 = rand();
        int y1 = rand();
        int x2 = rand();
        int y2 = rand();
        if (x2 < x1)
            swap(x1, x2);
        if (y2 < y1)
            swap(y1, y2);
        s.query(x1, y1, x2, y2);
    }

    cout << "All tests passed" << endl;
    return 0;
}
