/*
 * Binary indexed tree.
 * Remember that i & -i gives the last bit in an integer.
 * Tree nodes start at index 1.
 * v := underlying array
 */
#include <iostream>
#include <algorithm>
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
    bi_tree<int> test(5);
    test.increase(0, 1);
    test.increase(1, 2);
    test.increase(2, 3);
    test.increase(3, 4);
    test.increase(4, 5);
    test.increase(4, 5);
    cout << test.prefix(0) << endl; // 1
    cout << test.prefix(3) << endl; // 10
    cout << test.prefix(4) << endl; // 20
    cout << test.range(1, 3) << endl; // 9
    cout << test.range(4, 4) << endl; // 10
    return 0;
}
