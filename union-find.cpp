/*
 * Union-find data structure for integers 0 to n-1.
 * https://en.wikipedia.org/wiki/Disjoint-set_data_structure
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
using namespace std;

struct union_find {
    vector<int> sizes, ids;
    vector<list<int>> sets;

    union_find(int n): sizes(n, 1), ids(n), sets(n) {
        for (int i = 0; i < n; i++) {
            ids[i] = i;
            sets[i] = list<int>(1, i);
        }
    }

    int find(int e) { return ids[e]; }

    void merge(int e1, int e2) {
        int s1 = ids[e1], s2 = ids[e2];
        if (s1 == s2) return;
        // merge into larger set
        if (sets[s2].size() > sets[s1].size()) swap(s1, s2);
        for (int e : sets[s2]) ids[e] = s1;
        sets[s1].splice(sets[s1].end(), sets[s2]);
        sizes[s1] += sizes[s2];
    }

    int count() {
        int ret = 0;
        for (list<int> set : sets)
            if (set.size() > 0)
                ret++;
        return ret;
    }
};

int main() {
    union_find test(10);
    test.merge(1, 5);
    test.merge(1, 3);
    test.merge(3, 3);
    cout << (test.count() == 8 ? "pass" : "fail") << endl;
}
