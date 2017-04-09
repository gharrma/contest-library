/*
 * Binary indexed tree.
 * Remember that i & -i gives the last bit in an integer.
 * Tree nodes start at index 1.
 * v := underlying array
 */
#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cassert>
using namespace std;

template <typename T>
struct bit {
    int n;
    vector<T> v; // Use unordered_map<ll,T> if needed.

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
            if (i + step < v.size() && v[i + step] < x)
                i += step, x -= v[i];
        return i;
    }
};

int main() {
    int n = 100;
    vector<int> v(n);
    bit<int> b(n);
    int total_val = 0;
    for (int t = 0; t < 1000000; ++t) {
        int r = rand() % 3;
        if (r == 0) {
            int i = rand() % n, val = rand() % 100;
            v[i] += val;
            b.update(i, val);
            total_val += val;
        } else if (r == 1) {
            int l = rand() % n;
            int r = rand() % n;
            if (r < l)
                swap(l, r);
            int sum = accumulate(v.begin() + l, v.begin() + r + 1, 0);
            assert(b.query(l, r) == sum);
        } else {
            vector<int> p(n);
            p[0] = v[0];
            for (int i = 1; i < n; ++i)
                p[i] = p[i-1] + v[i];
            int x = rand() % (total_val + total_val / 2 + 1);
            int idx = distance(p.begin(), lower_bound(p.begin(), p.end(), x));
            assert(b.lower_bound(x) == idx);
        }
    }
    cout << "All tests passed" << endl;
    return 0;
}
