#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

template <typename T>
struct matrix {
    int n;
    vector<T> v;

    matrix<T>(int n, T fill = T(0), T diag = T(0))
        : n(n), v(n * n, fill)
    {
        for (int i = 0; i < n; i++)
            (*this)(i, i) = diag;
    }

    matrix<T>(int n, initializer_list<T> l): n(n), v(l) { v.resize(n * n); }

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

    bool operator==(const matrix<T>& b) const { return v == b.v; }
    bool operator!=(const matrix<T>& b) const { return v != b.v; }
};

template <typename T>
ostream& operator<<(ostream& os, const matrix<T>& m) {
    for (int i = 0; i < m.n; ++i) {
        if (i) cout << endl;
        for (int j = 0; j < m.n; ++j) {
            if (j) cout << " ";
            cout << m.v[m.n * i + j];
        }
    }
    return os;
}

int main() {
    matrix<int> a(2, {1, 2, 3, 4}), b(2, {4, 3, 2, 1}), id(2, 0, 1);
    auto c = id * a * b * id;
    bool pass = true;
    pass &= c == matrix<int>(2, {8, 5, 20, 13});
    pass &= a.pow(3) == matrix<int>(2, {37, 54, 81, 118});
    cout << (pass ? "All tests passed" : "Test failed") << endl;
    return pass ? 0 : 1;
}
