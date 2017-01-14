/*
 * Segment tree with arbitrary value type and operation.
 * Represented as a perfect binary tree, identity-initialized.
 * Root is 1. Parent of node i is i/2. Children of node i are 2*i and 2*i+1.
 * s := size (number of leaves)
 * v := underlying array
 * f := group operation (not necessarily commutative)
 * id := group identity
 */
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

template <typename T>
struct seg_tree {
    size_t s;
    vector<T> v;
    function<T(T,T)> f;
    T id;

    seg_tree<T>(size_t n,
                function<T(T,T)> f = [] (T l, T r) {return l + r;},
                T id = T())
        : id(id), f(f)
    {
        s = 1;
        while (s < n)
            s <<= 1;
        v.resize(2*s, id);
    }

    void update(size_t i, T t) {
        i += s;
        v[i] = t;
        for (i /= 2; i > 0; i /= 2)
            v[i] = f(v[2*i], v[2*i+1]);
    }

    T query(size_t i, size_t j) {
        i += s, j += s;
        T l = id, r = id;
        for (; i <= j; i /= 2, j /= 2) {
            if (i % 2 == 1) l = f(l, v[i++]);
            if (j % 2 == 0) r = f(v[j--], r);
        }
        return f(l, r);
    }
};

int main() {
    seg_tree<int> test(5, [] (int l, int r) {return max(l,r);});
    test.update(0, 3);
    test.update(1, 2);
    test.update(2, 4);
    test.update(3, 1);
    test.update(4, 5);
    cout << test.query(0, 2) << endl; // 4
    cout << test.query(3, 4) << endl; // 5
    cout << test.query(1, 3) << endl; // 4
    return 0;
}
