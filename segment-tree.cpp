/*
 * Segment tree data structure with arbitrary value type and operation.
 * Represented as perfect binary tree, identity-initialized.
 * Root is 1. Node i parent at i/2. Node i children at 2*i and 2*i + 1.
 */
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

template<typename T> struct seg_tree {
    size_t size;
    vector<T> nodes;
    function<T(T,T)> f;
    T id;

    seg_tree<T>(size_t n,
                function<T(T,T)> f = [] (T l, T r) {return l + r;},
                T id = T())
        : id(id), f(f)
    {
        size = 1;
        while (size < n)
            size <<= 1;
        nodes.resize(2*size, id);
    }

    void update(size_t i, T v) {
        i += size;
        nodes[i] = v;
        while (i > 1) {
            i /= 2;
            nodes[i] = f(nodes[2*i], nodes[2*i + 1]);
        }
    }

    T query(size_t i, size_t j) {
        i += size; j += size;
        T l = id, r = id;
        while (i <= j) {
            if (i % 2 == 1) l = f(l, nodes[i++]);
            if (j % 2 == 0) r = f(nodes[j--], r);
            i /= 2; j /= 2;
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
