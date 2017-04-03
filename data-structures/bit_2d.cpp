/*
 * 2-dimensional binary indexed tree.
 * Remember that i & -i gives the last bit in an integer.
 * Tree nodes start at index 1.
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
using namespace std;

template <typename T>
struct bit_2d {
    vector<vector<T>> v;

    bit_2d(int x, int y): v(x+1, vector<T>(y+1)) {}

    void update(int x, int y, T t) {
        for (int i = x + 1; i < v.size(); i += i & -i)
            for (int j = y + 1; j < v[i].size(); j += j & -j)
                v[i][j] += t;
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

int main() {
    int n = 10;
    vector<vector<int>> v(n, vector<int>(n));
    bit_2d<int> b(n, n);
    for (int t = 0; t < 1000000; ++t) {
        if (rand() % 2) {
            int x = rand() % n, y = rand() % n, val = rand() % 100;
            v[x][y] += val;
            b.update(x, y, val);
        } else {
            int x1 = rand() % n;
            int y1 = rand() % n;
            int x2 = rand() % n;
            int y2 = rand() % n;
            if (x2 < x1)
                swap(x1, x2);
            if (y2 < y1)
                swap(y1, y2);
            int sum = 0;
            for (int i = x1; i <= x2; ++i)
                for (int j = y1; j <= y2; ++j)
                    sum += v[i][j];
            assert(b.query(x1, y1, x2, y2) == sum);
        }
    }
    cout << "All tests passed" << endl;
    return 0;
}
