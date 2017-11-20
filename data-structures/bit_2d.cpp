/*
 * 2-dimensional binary indexed tree.
 */
template <typename T>
struct bit_2d {
    int n, m;
    vector<vector<T>> v;

    bit_2d(int n, int m): n(n+1), m(m+1), v(n+1, vector<T>(m+1)) {}

    void update(int x, int y, T t) {
        for (int i = x + 1; i < n; i += i & -i) {
            for (int j = y + 1; j < m; j += j & -j) {
                v[i][j] += t;
            }
        }
    }

    T query(int x, int y) {
        T sum = 0;
        for (int i = x + 1; i > 0; i -= i & -i)
            for (int j = y + 1; j > 0; j -= j & -j)
                sum += v[i][j];
        return sum;
    }

    T query(int x1, int y1, int x2, int y2) {
        if (x2 < x1 || y2 < y1)
            return 0;
        return query(x2, y2) - query(x2, y1-1)
                             - query(x1-1, y2) + query(x1-1, y1-1);
    }
};
