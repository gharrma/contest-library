/*
 * Binary indexed tree.
 * Remember that i & -i gives the last bit in an integer.
 * Tree nodes start at index 1.
 */
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

template <typename T>
struct bi_tree{
    vector<T> v;

    bi_tree<T>(int s): v(s+1) {}

    void inc(int i, T t) {
        for (++i; i < v.size(); i += i & -i)
            v[i] += t;
    }

    T query(int i) {
        T sum = 0;
        for (++i; i > 0; i -= i & -i)
            sum += v[i];
        return sum;
    }

    T query(int l, int r) {
        return query(r) - query(l-1);
    }
};

int main() {
    bi_tree<int> test(5);
    test.inc(0, 1);
    test.inc(1, 2);
    test.inc(2, 3);
    test.inc(3, 4);
    test.inc(4, 5);
    test.inc(4, 5);
    cout << test.query(0) << endl; // 1
    cout << test.query(3) << endl; // 10
    cout << test.query(4) << endl; // 20
    cout << test.query(1, 3) << endl; // 9
    cout << test.query(4, 4) << endl; // 10
    return 0;
}
