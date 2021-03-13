#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
using namespace std;

// Implementation notes:
//
// n := size
// id := identity element
// op := associative binary operation
//
// v represents a binary tree rooted at 1. The children of node i are at
// 2*i and 2*i+1. The parent of node i is at i/2. The last bit of a node index
// tells you whether it is the left or right child of its parent. The leaves
// of the tree are at [n,2*n). The leaves store values while the parents store
// accumulations over their children. To implement set(i, x) we update the
// corresponding leaf node along with its O(lg n) ancestors. To implement
// accumulate(l, r) we iterate through the ancestors of leaves l and r to find
// the O(lg n) segments which perfectly cover the range [l,r] without overlap.

template <typename T, typename AssociativeOp>
struct seg_tree {
    T id;
    AssociativeOp op;
    int n;
    vector<T> v;

    seg_tree(int n, T id, AssociativeOp op)
        : n(n), id(id), op(op), v(2*n, id) {}

    // Set the value at index i in O(lg n).
    void set(int i, T x) {
        assert(0 <= i && i < n);
        i += n;
        v[i] = x;
        for (i /= 2; i > 0; i /= 2) {
            v[i] = op(v[2*i], v[2*i+1]);
        }
    }

    // Fold over [l,r] in O(lg n).
    T accumulate(int l, int r) {
        assert(0 <= l && r < n);
        T a = id, b = id;
        for (l += n, r += n; l <= r; l /= 2, r /= 2) {
            if (l % 2 == 1) a = op(a, v[l++]);
            if (r % 2 == 0) b = op(v[r--], b);
        }
        return op(a, b);
    }

    // --- Begin optional methods ---

    // Get the value at index i.
    const T& get(int i) const {
        assert(0 <= i && i < n);
        return v[i+n];
    }

    // Set all values in O(n).
    template <typename It>
    void set_all(It begin, It end) {
        assert(distance(begin, end) <= n);
        copy(begin, end, v.begin() + n);
        for (int i = n-1; i > 0; --i) {
            v[i] = op(v[2*i], v[2*i+1]);
        }
    }
};

int main() {
    for (int n = 1; n <= 20; ++n) {
        vector<int> v(n, numeric_limits<int>::min());
        for (int i = 0; i < n; ++i) {
            v[i] = rand() % 100;
        }

        seg_tree s(n,
                   numeric_limits<int>::min(),
                   [](int a, int b) { return max(a, b); });
        s.set_all(v.begin(), v.end());

        for (int t = 0; t < 1000000; ++t) {
            if (rand() % 2) {
                int i = rand() % n, val = rand() % 100;
                v[i] = val;
                s.set(i, val);
                assert(s.get(i) == v[i]);
            } else {
                int l = rand() % n, r = rand() % n;
                if (r < l)
                    swap(l, r);
                int max_elem = *max_element(v.begin() + l, v.begin() + r + 1);
                assert(s.accumulate(l, r) == max_elem);
            }
        }
    }

    cout << "All tests passed" << endl;
    return 0;
}
