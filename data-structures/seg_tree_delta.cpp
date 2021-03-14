#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cassert>
using namespace std;

// Like seg_tree, but supports incremental updates.
// This is useful when the values are large objects (e.g. sets) that
// can only change incrementally (e.g. adding a new element to a set).
// When applying an update function f to an element, f must be commutative
// with respect to the (implicit) binary operator for this segment tree.
// That is, it must be true that f(a)*b = f(a*b).
template <typename T>
struct seg_tree_delta {
    int n;
    vector<T> v;

    seg_tree_delta(int n, T id): n(n), v(2*n, id) {}

    // Apply f to a value and its O(lg n) ancestors.
    template <typename F>
    void update(int i, F f) {
        assert(i <= i && i < n);
        for (i += n; i > 0; i /= 2) {
            f(v[i]);
        }
    }

    // Visit the O(lg n) segments covering the range [l,r].
    template <typename F>
    void visit(int l, int r, F f) {
        assert(0 <= l && r < n);
        for (l += n, r += n; l <= r; l /= 2, r /= 2) {
            if (l % 2 == 1) f(v[l++]);
            if (r % 2 == 0) f(v[r--]);
        }
    }
};

int main() {
    int n = 100;
    vector<unordered_map<int,int>> v(n);
    seg_tree_delta s(n, unordered_map<int,int>());
    for (int t = 0; t < 1000000; ++t) {
        int val = rand() % 100;
        if (rand() % 2) {
            int i = rand() % n;
            ++v[i][val];
            s.update(i, [&](auto& m) { ++m[val]; });
        } else {
            int l = rand() % n;
            int r = rand() % n;
            if (r < l) swap(l, r);
            int expected = 0;
            for (int i = l; i <= r; ++i) expected += v[i][val];
            int res = 0;
            s.visit(l, r, [&](const auto& m) { if (m.count(val)) res += m.at(val); });
            assert(res == expected);
        }
    }
    cout << "All tests passed" << endl;
    return 0;
}
