/*
 * 2-dimensional coordinate-compressed Fenwick tree.
 * Regardless of coordinate magnitudes, preprocessing is
 *   O(n lg n) and updates/queries are O(lg n lg n).
 * Faster than sparse Fenwick tree, but offline rather than online.
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
        for (const auto& p : pts)
            xs.push_back(p.first);
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        ys.resize(xs.size() + 1);
        v.resize(xs.size() + 1);

        // Compress second component.
        for (const auto& p : pts)
            for (auto i = idx(xs, p.first); i < v.size(); i += i & -i)
                ys[i].push_back(p.second);
        for (int i = 1; i < v.size(); ++i) {
            sort(ys[i].begin(), ys[i].end());
            ys[i].erase(unique(ys[i].begin(), ys[i].end()), ys[i].end());
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
    cout << "All tests passed" << endl;
    return 0;
}
