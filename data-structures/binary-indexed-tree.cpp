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
    vector<T> v;

    bit<T>(size_t n): v(n+1) {}

    void increase(size_t i, T t) {
        for (++i; i < v.size(); i += i & -i)
            v[i] += t;
    }

    T prefix(size_t i) {
        T sum = 0;
        for (++i; i > 0; i -= i & -i)
            sum += v[i];
        return sum;
    }

    T range(size_t l, size_t r) {
        return r < l ? 0 : prefix(r) - prefix(l-1);
    }

    size_t lower_bound(T x) {
        size_t i = 0, mask = 1;
        while (mask < v.size())
            mask <<= 1;
        for (; mask > 0; mask >>= 1)
            if (i + mask < v.size() && v[i + mask] < x)
                i += mask, x -= v[i];
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
            b.increase(i, val);
            total_val += val;
        } else if (r == 1) {
            size_t l = rand() % n, r = rand() % n;
            if (r < l) swap(l, r);
            int sum = accumulate(v.begin() + l, v.begin() + r + 1, 0);
            assert(b.range(l, r) == sum);
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
