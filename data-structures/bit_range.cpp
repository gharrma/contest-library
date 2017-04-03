/*
 * Binary indexed tree, supporting range updates and point queries.
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

int main() {
    int n = 100;
    vector<int> v(n);
    bit_range<int> b(n);
    for (int t = 0; t < 1000000; ++t) {
        int r = rand() % 2;
        if (r == 0) {
            int l = rand() % n, r = rand() % n, val = rand() % 100;
            if (r < l)
                swap(l, r);
            for (int i = l; i <= r; ++i)
                v[i] += val;
            b.update(l, r, val);
        } else {
            int i = rand() % n;
            assert(b.query(i) == v[i]);
        }
    }
    cout << "All tests passed" << endl;
    return 0;
}
