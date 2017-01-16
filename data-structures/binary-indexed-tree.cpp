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
using namespace std;

template <typename T>
struct bi_tree {
    vector<T> v;

    bi_tree<T>(size_t n): v(n+1) {}

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
};

int main() {
    int n = 100;
    vector<int> v(n);
    bi_tree<int> b(n);
    for (int t = 0; t < 1e6; ++t) {
        if (rand() % 2) {
            int i = rand() % n, val = rand() % 100;
            v[i] += val;
            b.increase(i, val);
        } else {
            size_t l = rand() % n, r = rand() % n;
            if (r < l) swap(l, r);
            int sum = accumulate(v.begin() + l, v.begin() + r + 1, 0);
            if (b.range(l, r) != sum) {
                cout << "Test failed" << endl;
                return 1;
            }
        }
    }
    cout << "All tests passed" << endl;
    return 0;
}
