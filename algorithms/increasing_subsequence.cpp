/*
 * Returns a longest increasing subsequence in O(n lg n).
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

template <typename Seq>
vector<typename Seq::value_type>
increasing_subsequence(const Seq& seq, bool strict = true) {
    auto len = distance(begin(seq), end(seq));
    vector<int> best;
    vector<int> pred(len);
    auto cmp = [&](int i, int j) { return seq[i] < seq[j]; };
    for (int i = 0; i < len; ++i) {
        auto find = strict
            ? lower_bound(best.begin(), best.end(), i, cmp)
            : upper_bound(best.begin(), best.end(), i, cmp);
        pred[i] = find == best.begin() ? -1 : *prev(find);
        if (find == best.end()) {
            best.push_back(i);
        } else {
            *find = i;
        }
    }
    vector<typename Seq::value_type> res(best.size());
    int i = best.size();
    for (int k = best.back(); k != -1; k = pred[k])
        res[--i] = seq[k];
    return res;
}

int main() {
    assert(increasing_subsequence(vector<int>({1,2,3,4,5,6,7,8,9}))
                               == vector<int>({1,2,3,4,5,6,7,8,9}));

    assert(increasing_subsequence(vector<int>({1,5,3,7,9,1,2}))
                               == vector<int>({1,3,7,9}));

    assert(increasing_subsequence(vector<int>({1,5,6,2,3,4}))
                               == vector<int>({1,2,3,4}));

    assert(increasing_subsequence(vector<int>({5,4,3,2,1}))
                               == vector<int>({1}));

    assert(increasing_subsequence(vector<int>({1,1,1,5,5,2,2,2}))
                               == vector<int>({1,2}));

    assert(increasing_subsequence(vector<int>({1,1,1,5,5,2,2,2}), false)
                               == vector<int>({1,1,1,2,2,2}));

    cout << "All tests passed" << endl;
}
