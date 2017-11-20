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
constexpr double eps = 1e-9;

template <typename F>
int binary_search_lr(int n, F f) {
    int l = 0, r = n-1;
    while (l < r) {
        int m = (l + r) / 2;
        f(m) ? r = m : l = m+1;
    }
    return l;
}

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
