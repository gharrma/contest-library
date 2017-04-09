/*
 * 2-dimensional segment tree supporting operations over
 *   rectangular regions.
 * Add values at specific coordinates, then call build().
 * The data structure is static once built.
 * Regardless of coordinate magnitudes, preprocessing is
 *   O(n lg n lg n) and queries are O(lg n lg n).
 *
 * s := size (number of leaves)
 * pts := vector of all points and associated values
 * xs := sorted vector of distinct x-coordinates
 * ys[i] := sorted vector of distinct y-coordinates of
 *          points with the ith lowest x-coordinate
 * bt[i] := the fenwick tree at node i
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
using namespace std;
using ll = long long;

template <typename T>
struct bit {
    vector<T> v;

    bit(int n): v(n+1) {}

    void update(int i, T t) {
        for (++i; i < v.size(); i += i & -i) {
            v[i] += t;
        }
    }

    T query(int i) {
        T sum = 0;
        for (++i; i > 0; i -= i & -i)
            sum += v[i];
        return sum;
    }
};

template <typename T>
struct seg_tree_2d {
    struct pt {
        ll x, y; T val;
        bool operator<(const pt& o) const {
            return tie(x, y, val) < tie(o.x, o.y, o.val);
        }
    };

    int s;
    vector<pt> pts;
    vector<ll> xs;
    vector<vector<ll>> ys;
    vector<bit<T>> bt;

    void add(ll x, ll y, T val) {
        pts.push_back({x, y, val});
    }

    void build() {
        // Compress first component.
        sort(pts.begin(), pts.end());
        for (auto& p : pts) {
            if (xs.empty() || p.x != xs.back())
                xs.push_back(p.x);
            p.x = xs.size() - 1;
        }

        for (s = 1; s < xs.size(); )
            s <<= 1;
        ys.resize(2*s);
        bt.resize(2*s, 0);

        // Build tree.
        for (const auto& p : pts) {
            auto i = p.x + s;
            if (ys[i].empty() || p.y != ys[i].back()) {
                ys[i].push_back(p.y);
            }
        }
        for (int i = s; i < 2*s; ++i) {
            bt[i] = bit<T>(ys[i].size());
        }
        for (int i = s - 1; i >= 0; --i) {
            set_union(ys[2*i].begin(),   ys[2*i].end(),
                      ys[2*i+1].begin(), ys[2*i+1].end(),
                      back_inserter(ys[i]));
            bt[i] = bit<T>(ys[i].size());
        }
        for (const auto& p : pts) {
            for (int i = p.x + s; i > 0; i /= 2) {
                bt[i].update(at_least(ys[i], p.y), p.val);
            }
        }
    }

    int query(ll l, ll r, ll y) {
        l = at_least(xs, l) + s;
        r = at_most (xs, r) + s;
        T res = 0;
        for (; l <= r; l /= 2, r /= 2) {
            if (l % 2 == 1) res += bt[l].query(at_most(ys[l], y)), ++l;
            if (r % 2 == 0) res += bt[r].query(at_most(ys[r], y)), --r;
        }
        return res;
    }

    T query(ll l, ll r, ll y1, ll y2) {
        return y1 < y2 ? query(l, r, y2) - query(l, r, y1-1) : 0;
    }

    int at_least(const vector<ll>& v, ll val) {
        auto lb = lower_bound(v.begin(), v.end(), val);
        return distance(v.begin(), lb);
    }

    int at_most(const vector<ll>& v, ll val) {
        auto ub = upper_bound(v.begin(), v.end(), val);
        return distance(v.begin(), ub) - 1;
    }
};

int main() {
    seg_tree_2d<int> s;
    s.add(0, 1, 1);
    s.add(0, 15, 1);
    s.add(7, 7, 1);
    s.add(7, 7, 1);
    s.add(7, 7, 3);
    s.add(12, 13, 1);
    s.add(123456789, 987654321, 1);
    s.add(123456789, 999999999, 2);
    s.add(999999999, 123456789, 1);
    s.build();
    assert(s.query(0,0,0) == 0);
    assert(s.query(0,0,1) == 1);
    assert(s.query(0,1,1) == 1);
    assert(s.query(0,1,15) == 2);
    assert(s.query(0,1,7,32) == 1);
    assert(s.query(0,30,15) == 8);
    assert(s.query(12345,999999998,987654321) == 1);
    assert(s.query(12345,999999998,1000000000) == 3);
    assert(s.query(0,1000000000,1000000000) == 12);
    cout << "All tests passed" << endl;
    return 0;
}
