/*
 * Union-find data structure for integers 0 to n-1.
 * https://en.wikipedia.org/wiki/Disjoint-set_data_structure
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
using namespace std;

vector<int> set_sizes, set_ids;
vector<list<int>> sets;

void set_init(int n) {
    set_sizes = vector<int>(n, 1);
    set_ids = vector<int>(n);
    sets = vector<list<int>>(n);
    for (int i = 0; i < n; i++) {
        set_ids[i] = i;
        sets[i] = list<int>(1, i);
    }
}

void set_merge(int e1, int e2) {
    int s1 = set_ids[e1], s2 = set_ids[e2];
    if (s1 == s2)
        return;

    // ensure |s1| >= |s2|
    if (sets[s2].size() > sets[s1].size())
        swap(s1, s2);

    // merge into s1
    for (int e : sets[s2])
        set_ids[e] = s1;
    sets[s1].splice(sets[s1].end(), sets[s2]);
    set_sizes[s1] += set_sizes[s2];
}

int set_count() {
    int ret = 0;
    for (list<int> set : sets)
        if (set.size() > 0)
            ret++;
    return ret;
}

int main() {
    set_init(10);
    set_merge(1, 5);
    set_merge(1, 3);
    set_merge(3, 3);
    cout << (set_count() == 8 ? "pass" : "fail") << endl;
}
