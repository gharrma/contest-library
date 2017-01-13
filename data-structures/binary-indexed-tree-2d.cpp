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
struct bi_tree_2d {
    vector<vector<T>> v;

    bi_tree_2d<T>(size_t x, size_t y): v(x+1, vector<T>(y+1)) {}

    void increase(size_t x, size_t y, T t) {
        for (size_t i = x + 1; i < v.size(); i += i & -i)
            for (size_t j = y + 1; j < v[i].size(); j += j & -j)
                v[i][j] += t;
    }

    T prefix(size_t x, size_t y) {
        T sum = 0;
        for (size_t i = x + 1; i > 0; i -= i & -i)
            for (size_t j = y + 1; j > 0; j -= j & -j)
                sum += v[i][j];
        return sum;
    }

    T range(size_t x1, size_t y1, size_t x2, size_t y2) {
        return prefix(x2, y2) - prefix(x2, y1-1)
                              - prefix(x1-1, y2) + prefix(x1-1, y1-1);
    }
};

int main() {
    bi_tree_2d<int> test(5, 10);
    test.increase(0, 0, 1);
    test.increase(1, 1, 2);
    test.increase(2, 3, 3);
    test.increase(3, 1, 4);
    test.increase(4, 4, 5);
    test.increase(4, 4, 5);
    cout << test.prefix(0, 0) << endl; // 1
    cout << test.prefix(3, 0) << endl; // 1
    cout << test.prefix(3, 3) << endl; // 10
    cout << test.prefix(4, 4) << endl; // 20
    cout << test.range(1, 1, 2, 3) << endl; // 5
    cout << test.range(0, 0, 4, 4) << endl; // 20
    return 0;
}
