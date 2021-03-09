/*
 * Binary search finds the smallest integer x in [0,n) for which f(x)
 * is true, else n if f(x) is never true. O(lg n).
 *
 * Ternary search finds the x in [l,r] that maximizes f(x) assuming
 * that f is increasing then decreasing. O(lg n). The search
 * interval is (l,r) in the floating-point case.
 *
 * l := left
 * r := right
 * m := mid
 * s := step size
 */
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
using namespace std;
constexpr double eps = 1e-9;

// Returns leftmost i in [0, n) such that f(i) == true. Else returns n.
template <typename F>
int binary_search_lr(int n, F f) {
    int l = 0, r = n;
    while (l < r) {
        int m = (l + r) / 2;
        f(m) ? r = m : l = m+1;
    }
    return l;
}

// Returns leftmost i in [0, n) such that f(i) == true. Else returns n.
template <typename F>
int binary_search_bit(int n, F f) {
    int l = -1, s = 1;
    while (s < n)
        s <<= 1;
    for (; s > 0; s >>= 1)
        if (l + s < n && !f(l + s))
            l += s;
    return l + 1;
}

template <typename F>
int ternary_search(int l, int r, F f) {
    while (l < r) {
        int m = (l + r) / 2;
        f(m) > f(m+1) ? r = m : l = m+1;
    }
    return l;
}

template <typename F>
double ternary_search(double l, double r, F f) {
    while (r - l > eps) {
        auto third = (r - l) / 3;
        auto m1 = l + third;
        auto m2 = r - third;
        f(m1) > f(m2) ? r = m2 : l = m1;
    }
    return (l + r) / 2;
}

int main() {
    int n = 10000;
    vector<int> v(n);
    for (int i = 0; i < n; ++i)
        v[i] = rand();
    sort(v.begin(), v.end());
    for (int i = 0; i < 1000000; ++i) {
        int k = rand();
        auto cmp = [&](int x) { return v[x] >= k; };
        int lr = binary_search_lr(v.size(), cmp);
        int bit = binary_search_bit(v.size(), cmp);
        int lb = distance(v.begin(), lower_bound(v.begin(), v.end(), k));
        assert(lr == lb);
        assert(bit == lb);
    }

    auto fmax = ternary_search(-1000., 1000., [](double x) {
        return -x*x + x + 1;
    });
    assert(fabs(fmax - .5) < 1e-6);

    auto dmax = ternary_search(-1000, 1000, [](int x) {
        return -x*x + 2*x + 1;
    });
    assert(dmax == 1);

    cout << "All tests passed" << endl;
    return 0;
}
