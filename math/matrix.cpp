#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

template <typename T>
struct matrix {
    int n;
    vector<T> v;

    matrix<T> (int n, T fill = T(0), T diag = T(0))
        : n(n), v(n * n, fill)
    {
        for (int i = 0; i < n; i++)
            (*this)(i, i) = diag;
    }

    T&       operator()(int i, int j)       { return v[n * i + j]; }
    const T& operator()(int i, int j) const { return v[n * i + j]; }

    matrix<T> operator*(const matrix<T>& b) const {
        matrix<T> ret(n);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                for (int k = 0; k < n; ++k)
                    ret(i, j) += (*this)(i, k) * b(k, j);
        return ret;
    }

    matrix<T> pow(ll e) const {
        if (e == 0) return matrix<T>(n, T(0), T(1));
        matrix<T> rec = (*this * *this).pow(e / 2);
        return e % 2 ? rec * *this : rec;
    }
};

int main() {
    return 0;
}
