/*
 * 2-dimensional binary indexed tree.
 * Remember that i & -i gives the last bit in an integer.
 * Tree nodes start at index 1.
 */
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

template <typename T>
struct bi_tree{
    vector<vector<T>> v;

    bi_tree<T>(size_t x, size_t y): v(x+1, vector<T>(y+1)) {}

    void inc(size_t x, size_t y, T t) {
        for (size_t i = x + 1; i < v.size(); i += i & -i)
            for (size_t j = y + 1; j < v[i].size(); j += j & -j)
                v[i][j] += t;
    }

    T query(size_t x, size_t y) {
        T sum = 0;
        for (size_t i = x + 1; i > 0; i -= i & -i)
            for (size_t j = y + 1; j > 0; j -= j & -j)
                sum += v[i][j];
        return sum;
    }

    T query(size_t x1, size_t y1, size_t x2, size_t y2) {
        return query(x2, y2) - query(x2, y1-1)
                             - query(x1-1, y2) + query(x1-1, y1-1);
    }
};

int main() {
    bi_tree<int> test(5, 10);
    test.inc(0, 0, 1);
    test.inc(1, 1, 2);
    test.inc(2, 3, 3);
    test.inc(3, 1, 4);
    test.inc(4, 4, 5);
    test.inc(4, 4, 5);
    cout << test.query(0, 0) << endl; // 1
    cout << test.query(3, 0) << endl; // 1
    cout << test.query(3, 3) << endl; // 10
    cout << test.query(4, 4) << endl; // 20
    cout << test.query(1, 1, 2, 3) << endl; // 5
    cout << test.query(0, 0, 4, 4) << endl; // 20
    return 0;
}
